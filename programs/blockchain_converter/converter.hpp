#pragma once

#include <map>
#include <mutex>
#include <string>
#include <queue>
#include <set>
#include <thread>
#include <vector>
#include <utility>
#include <atomic>
#include <array>
#include <functional>

#include <fc/exception/exception.hpp>
#include <fc/optional.hpp>
#include <fc/time.hpp>

#include <hive/chain/full_block.hpp>
#include <hive/chain/full_transaction.hpp>

#include <hive/protocol/config.hpp>
#include <hive/protocol/types.hpp>
#include <hive/protocol/authority.hpp>
#include <hive/protocol/operations.hpp>
#include <hive/protocol/block.hpp>

#include <boost/lockfree/stack.hpp>

#define HIVE_BC_TIME_BUFFER 10

namespace appbase {
  class application;
}

#define HIVE_BC_SAFETY_TIME_GAP (HIVE_BLOCK_INTERVAL * HIVE_BC_TIME_BUFFER)

namespace hive { namespace converter {

  namespace hp = hive::protocol;
  namespace hc = hive::chain;

  class blockchain_converter
  {
  public:
    bool increase_transaction_expiration_time = false;

  private:
    hp::private_key_type _private_key;
    hp::chain_id_type    chain_id;
    hp::signed_block*    current_block_ptr = nullptr;

    std::map< hp::authority::classification, hp::private_key_type > second_authority;

    std::queue< hp::authority > pow_keys;
    std::set< hp::account_name_type > accounts;

    std::set< hp::transaction_id_type > tapos_scope_tx_ids;

    void post_convert_transaction( hp::signed_transaction& trx );

    std::vector< std::thread > signers; // Transactions signers (defualt number is 1)

    typedef std::pair< size_t, hc::full_transaction_ptr > sig_stack_in_type;
    typedef size_t sig_stack_out_type;

    boost::lockfree::stack< sig_stack_in_type >  shared_signatures_stack_in;  // pair< trx index in block, signed transaction ptr to convert >
    boost::lockfree::stack< sig_stack_out_type > shared_signatures_stack_out; // pair< trx index in block, converted signature >

    std::queue< std::shared_ptr< hc::full_transaction_type > > pow_transactions;

    bool increase_block_size;

    std::atomic_bool        signers_exit;

    hp::block_id_type converter_head_block_id;
    hp::block_id_type mainnet_head_block_id;

    static const std::array< uint32_t, HIVE_NUM_HARDFORKS + 1 > hardfork_blocks;

    std::vector< hp::private_key_type > transaction_signing_keys;

    uint32_t cached_hf = 0;

    uint64_t total_tx_count = 0;

  public:
    /// All converted blocks will be signed using given private key
    blockchain_converter( const hp::private_key_type& _private_key, const hp::chain_id_type& chain_id, appbase::application& app, size_t signers_size = 1, bool increase_block_size = true );
    ~blockchain_converter();

    /// Sets previous id of the block to the given value and re-signs content of the block. Converts transactions. Returns current block id
    std::shared_ptr< hc::full_block_type > convert_signed_block( hp::signed_block& _signed_block, const hp::block_id_type& previous_block_id, const fc::time_point_sec& now_time, bool alter_time_in_visitor = false, uint64_t account_creation_fee = uint64_t(-1) );

    /// This function does not alter the internal converter state (like for example cached hardfork number or mainner head block id. This is just an utility function).
    /// If you are using it before HF17 make sure to use pop_helper_pow_transaction() and has_helper_pow_transaction() methods
    std::shared_ptr< hc::full_transaction_type > convert_signed_transaction( hp::signed_transaction& tx, const hp::block_id_type& previous_block_id, const std::function<void(hp::transaction&)>& apply_trx_expiration_offset, uint32_t block_offset = 0, uint64_t account_creation_fee = uint64_t(-1), bool enable_signing = false );

    bool has_helper_pow_transaction()const;
    std::shared_ptr< hc::full_transaction_type > pop_helper_pow_transaction();

    uint32_t calculate_transaction_expiration( uint32_t head_block_time, uint32_t block_timestamp = 0, uint32_t trx_expiration = 0, uint32_t block_offset = 0, uint32_t trx_time_offset = 0 )const;

    const hp::block_id_type& get_converter_head_block_id()const;
    const hp::block_id_type& get_mainnet_head_block_id()const;

    const uint64_t get_total_tx_count()const;
    uint32_t get_converter_head_block_num()const;

    void add_second_authority( hp::authority& _auth, hp::authority::classification type );

    void sign_transaction( hc::full_transaction_type& trx )const;

    const hp::private_key_type& get_second_authority_key( hp::authority::classification type )const;
    void set_second_authority_key( const hp::private_key_type& key, hp::authority::classification type );

    void add_pow_key( const hp::account_name_type& acc, const hp::public_key_type& key );
    void add_account( const hp::account_name_type& acc );
    bool has_account( const hp::account_name_type& acc )const;

    const hp::private_key_type& get_witness_key()const;
    const hp::chain_id_type& get_chain_id()const;

    /**
     * @brief Alters the current converter state by changing the saved mainnet head block id.
     *        This function can be used to "move in time", e.g. if you want to apply hardforks
     *        without running the actual block conversion. Useful in the initial checks like the chain id validation
     * @note Saved mainnet head block id is the original value of the `previous` struct member of the currently
     *       processed or the previously processed block
     */
    void touch( const hp::signed_block_header& _signed_header );
    void touch( const hp::block_id_type& id ); // Function override if you want to directly set your mainnet head block id

    // Should be called every time TaPoS value changes in your plugin
    void on_tapos_change();

    bool has_hardfork( uint32_t hf )const;
    static bool has_hardfork( uint32_t hf, uint32_t block_num );
    static bool has_hardfork( uint32_t hf, const hp::signed_block& _signed_block );

    uint32_t get_cached_hardfork()const;

    /**
     * @throws if there is no block being currently converted
     */
    const hp::signed_block& get_current_block()const;

    bool block_size_increase_enabled()const;

    // Needs to be called before signing transactions
    void apply_second_authority_keys();
  };

  class convert_operations_visitor
  {
  private:
    blockchain_converter& converter;
    uint32_t block_offset;
    uint64_t account_creation_fee;

  public:
    typedef hp::operation result_type;

    convert_operations_visitor( blockchain_converter& converter, uint32_t block_offset, uint64_t account_creation_fee );

    const hp::account_create_operation& operator()( hp::account_create_operation& op )const;

    const hp::account_create_with_delegation_operation& operator()( hp::account_create_with_delegation_operation& op )const;

    const hp::claim_account_operation& operator()( hp::claim_account_operation& op )const;

    const hp::account_update_operation& operator()( hp::account_update_operation& op )const;

    const hp::account_update2_operation& operator()( hp::account_update2_operation& op )const;

    const hp::create_claimed_account_operation& operator()( hp::create_claimed_account_operation& op )const;

    const hp::custom_binary_operation& operator()( hp::custom_binary_operation& op )const;

    const hp::escrow_transfer_operation& operator()( hp::escrow_transfer_operation& op )const;

    const hp::limit_order_create_operation& operator()( hp::limit_order_create_operation& op )const;

    const hp::limit_order_create2_operation& operator()( hp::limit_order_create2_operation& op )const;

    const hp::pow_operation& operator()( hp::pow_operation& op )const;

    const hp::pow2_operation& operator()( hp::pow2_operation& op )const;

    const hp::request_account_recovery_operation& operator()( hp::request_account_recovery_operation& op )const;

    const hp::reset_account_operation& operator()( hp::reset_account_operation& op )const;

    const hp::recover_account_operation& operator()( hp::recover_account_operation& op )const;

    const hp::witness_update_operation& operator()( hp::witness_update_operation& op )const;

    const hp::witness_set_properties_operation& operator()( hp::witness_set_properties_operation& op )const;

    // No signatures modification ops
    template< typename T >
    const T& operator()( const T& op )const
    {
      FC_ASSERT( !op.is_virtual(), "block log should not contain virtual operations" );
      return op;
    }
  };

} } // hive::converter

#include <hive/protocol/get_config.hpp>
#include <hive/protocol/config.hpp>
#include <hive/protocol/asset.hpp>
#include <hive/protocol/types.hpp>
#include <hive/protocol/version.hpp>

namespace hive { namespace protocol {

fc::variant_object get_config( const std::string& treasury_name, const fc::sha256& chain_id )
{
  fc::mutable_variant_object result;

#ifdef IS_TEST_NET
  result["IS_TEST_NET"] = true;
  result["HIVE_INIT_PRIVATE_KEY"] = HIVE_INIT_PRIVATE_KEY;
  result["TESTNET_BLOCK_LIMIT"] = TESTNET_BLOCK_LIMIT;
#else
  result["IS_TEST_NET"] = false;
#endif

#ifdef HIVE_ENABLE_SMT
  result["HIVE_ENABLE_SMT"] = true;
  result["SMT_MAX_VOTABLE_ASSETS"] = SMT_MAX_VOTABLE_ASSETS;
  result["SMT_VESTING_WITHDRAW_INTERVAL_SECONDS"] = SMT_VESTING_WITHDRAW_INTERVAL_SECONDS;
  result["SMT_UPVOTE_LOCKOUT"] = SMT_UPVOTE_LOCKOUT;
  result["SMT_EMISSION_MIN_INTERVAL_SECONDS"] = SMT_EMISSION_MIN_INTERVAL_SECONDS;
  result["SMT_EMIT_INDEFINITELY"] = SMT_EMIT_INDEFINITELY;
  result["SMT_MAX_NOMINAL_VOTES_PER_DAY"] = SMT_MAX_NOMINAL_VOTES_PER_DAY;
  result["SMT_MAX_VOTES_PER_REGENERATION"] = SMT_MAX_VOTES_PER_REGENERATION;
  result["SMT_DEFAULT_VOTES_PER_REGEN_PERIOD"] = SMT_DEFAULT_VOTES_PER_REGEN_PERIOD;
  result["SMT_DEFAULT_PERCENT_CURATION_REWARDS"] = SMT_DEFAULT_PERCENT_CURATION_REWARDS;
#else
  result["HIVE_ENABLE_SMT"] = false;
#endif

  result["HIVE_CHAIN_ID"] = chain_id;
  result["HIVE_TREASURY_ACCOUNT"] = treasury_name;
  result["HIVE_BLOCKCHAIN_VERSION"] = HIVE_BLOCKCHAIN_VERSION;
  result["HIVE_INIT_PUBLIC_KEY_STR"] = HIVE_INIT_PUBLIC_KEY_STR;
  result["STEEM_CHAIN_ID"] = STEEM_CHAIN_ID;
  result["HIVE_CHAIN_ID"] = HIVE_CHAIN_ID;
  result["HIVE_ADDRESS_PREFIX"] = HIVE_ADDRESS_PREFIX;
  result["HIVE_GENESIS_TIME"] = HIVE_GENESIS_TIME;
  result["HIVE_MINING_TIME"] = HIVE_MINING_TIME;
  result["HIVE_CASHOUT_WINDOW_SECONDS"] = HIVE_CASHOUT_WINDOW_SECONDS;
  result["HIVE_CASHOUT_WINDOW_SECONDS_PRE_HF12"] = HIVE_CASHOUT_WINDOW_SECONDS_PRE_HF12;
  result["HIVE_CASHOUT_WINDOW_SECONDS_PRE_HF17"] = HIVE_CASHOUT_WINDOW_SECONDS_PRE_HF17;
  result["HIVE_SECOND_CASHOUT_WINDOW"] = HIVE_SECOND_CASHOUT_WINDOW;
  result["HIVE_MAX_CASHOUT_WINDOW_SECONDS"] = HIVE_MAX_CASHOUT_WINDOW_SECONDS;
  result["HIVE_UPVOTE_LOCKOUT_HF7"] = HIVE_UPVOTE_LOCKOUT_HF7;
  result["HIVE_UPVOTE_LOCKOUT_SECONDS"] = HIVE_UPVOTE_LOCKOUT_SECONDS;
  result["HIVE_UPVOTE_LOCKOUT_HF17"] = HIVE_UPVOTE_LOCKOUT_HF17;
  result["HIVE_MIN_ACCOUNT_CREATION_FEE"] = HIVE_MIN_ACCOUNT_CREATION_FEE;
  result["HIVE_MAX_ACCOUNT_CREATION_FEE"] = HIVE_MAX_ACCOUNT_CREATION_FEE;
  result["HIVE_OWNER_AUTH_RECOVERY_PERIOD"] = HIVE_OWNER_AUTH_RECOVERY_PERIOD;
  result["HIVE_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD"] = HIVE_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD;
  result["HIVE_OWNER_UPDATE_LIMIT"] = HIVE_OWNER_UPDATE_LIMIT;
  result["HIVE_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM"] = HIVE_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM;
  result["HIVE_INIT_SUPPLY"] = HIVE_INIT_SUPPLY;
  result["HIVE_HBD_INIT_SUPPLY"] = HIVE_HBD_INIT_SUPPLY;
  result["HIVE_PROPOSAL_MAINTENANCE_PERIOD"] = HIVE_PROPOSAL_MAINTENANCE_PERIOD;
  result["HIVE_PROPOSAL_MAINTENANCE_CLEANUP"] = HIVE_PROPOSAL_MAINTENANCE_CLEANUP;
  result["HIVE_DAILY_PROPOSAL_MAINTENANCE_PERIOD"] = HIVE_DAILY_PROPOSAL_MAINTENANCE_PERIOD;
  result["HIVE_GOVERNANCE_VOTE_EXPIRATION_PERIOD"] = HIVE_GOVERNANCE_VOTE_EXPIRATION_PERIOD;
  result["HIVE_GLOBAL_REMOVE_THRESHOLD"] = HIVE_GLOBAL_REMOVE_THRESHOLD;
  result["VESTS_SYMBOL"] = VESTS_SYMBOL;
  result["HIVE_SYMBOL"] = HIVE_SYMBOL;
  result["HBD_SYMBOL"] = HBD_SYMBOL;
  result["HIVE_BLOCKCHAIN_HARDFORK_VERSION"] = HIVE_BLOCKCHAIN_HARDFORK_VERSION;
  result["HIVE_100_PERCENT"] = HIVE_100_PERCENT;
  result["HIVE_1_PERCENT"] = HIVE_1_PERCENT;
  result["HIVE_1_BASIS_POINT"] = HIVE_1_BASIS_POINT;
  result["HIVE_BLOCK_INTERVAL"] = HIVE_BLOCK_INTERVAL;
  result["HIVE_BLOCKS_PER_YEAR"] = HIVE_BLOCKS_PER_YEAR;
  result["HIVE_BLOCKS_PER_DAY"] = HIVE_BLOCKS_PER_DAY;
  result["HIVE_START_VESTING_BLOCK"] = HIVE_START_VESTING_BLOCK;
  result["HIVE_START_MINER_VOTING_BLOCK"] = HIVE_START_MINER_VOTING_BLOCK;
  result["HIVE_INIT_MINER_NAME"] = HIVE_INIT_MINER_NAME;
  result["HIVE_NUM_INIT_MINERS"] = HIVE_NUM_INIT_MINERS;
  result["HIVE_INIT_TIME"] = HIVE_INIT_TIME;
  result["HIVE_MAX_WITNESSES"] = HIVE_MAX_WITNESSES;
  result["HIVE_MAX_VOTED_WITNESSES_HF0"] = HIVE_MAX_VOTED_WITNESSES_HF0;
  result["HIVE_MAX_MINER_WITNESSES_HF0"] = HIVE_MAX_MINER_WITNESSES_HF0;
  result["HIVE_MAX_RUNNER_WITNESSES_HF0"] = HIVE_MAX_RUNNER_WITNESSES_HF0;
  result["HIVE_MAX_VOTED_WITNESSES_HF17"] = HIVE_MAX_VOTED_WITNESSES_HF17;
  result["HIVE_MAX_MINER_WITNESSES_HF17"] = HIVE_MAX_MINER_WITNESSES_HF17;
  result["HIVE_MAX_RUNNER_WITNESSES_HF17"] = HIVE_MAX_RUNNER_WITNESSES_HF17;
  result["HIVE_HARDFORK_REQUIRED_WITNESSES"] = HIVE_HARDFORK_REQUIRED_WITNESSES;
  result["HIVE_MAX_TIME_UNTIL_EXPIRATION"] = HIVE_MAX_TIME_UNTIL_EXPIRATION;
  result["HIVE_MAX_MEMO_SIZE"] = HIVE_MAX_MEMO_SIZE;
  result["HIVE_MAX_PROXY_RECURSION_DEPTH"] = HIVE_MAX_PROXY_RECURSION_DEPTH;
  result["HIVE_VESTING_WITHDRAW_INTERVALS_PRE_HF_16"] = HIVE_VESTING_WITHDRAW_INTERVALS_PRE_HF_16;
  result["HIVE_VESTING_WITHDRAW_INTERVALS"] = HIVE_VESTING_WITHDRAW_INTERVALS;
  result["HIVE_VESTING_WITHDRAW_INTERVAL_SECONDS"] = HIVE_VESTING_WITHDRAW_INTERVAL_SECONDS;
  result["HIVE_MAX_WITHDRAW_ROUTES"] = HIVE_MAX_WITHDRAW_ROUTES;
  result["HIVE_MAX_PENDING_TRANSFERS"] = HIVE_MAX_PENDING_TRANSFERS;
  result["HIVE_SAVINGS_WITHDRAW_TIME"] = HIVE_SAVINGS_WITHDRAW_TIME;
  result["HIVE_SAVINGS_WITHDRAW_REQUEST_LIMIT"] = HIVE_SAVINGS_WITHDRAW_REQUEST_LIMIT;
  result["HIVE_VOTING_MANA_REGENERATION_SECONDS"] = HIVE_VOTING_MANA_REGENERATION_SECONDS;
  result["HIVE_MAX_VOTE_CHANGES"] = HIVE_MAX_VOTE_CHANGES;
  result["HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF6"] = HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF6;
  result["HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF20"] = HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF20;
  result["HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF21"] = HIVE_REVERSE_AUCTION_WINDOW_SECONDS_HF21;
  result["HIVE_EARLY_VOTING_SECONDS_HF25"] = HIVE_EARLY_VOTING_SECONDS_HF25;
  result["HIVE_MID_VOTING_SECONDS_HF25"] = HIVE_MID_VOTING_SECONDS_HF25;
  result["HIVE_MIN_VOTE_INTERVAL_SEC"] = HIVE_MIN_VOTE_INTERVAL_SEC;
  result["HIVE_VOTE_DUST_THRESHOLD"] = HIVE_VOTE_DUST_THRESHOLD;
  result["HIVE_DOWNVOTE_POOL_PERCENT_HF21"] = HIVE_DOWNVOTE_POOL_PERCENT_HF21;
  result["HIVE_DELAYED_VOTING_TOTAL_INTERVAL_SECONDS"] = HIVE_DELAYED_VOTING_TOTAL_INTERVAL_SECONDS;
  result["HIVE_DELAYED_VOTING_INTERVAL_SECONDS"] = HIVE_DELAYED_VOTING_INTERVAL_SECONDS;
  result["HIVE_MIN_ROOT_COMMENT_INTERVAL"] = HIVE_MIN_ROOT_COMMENT_INTERVAL;
  result["HIVE_MIN_REPLY_INTERVAL"] = HIVE_MIN_REPLY_INTERVAL;
  result["HIVE_MIN_REPLY_INTERVAL_HF20"] = HIVE_MIN_REPLY_INTERVAL_HF20;
  result["HIVE_MIN_COMMENT_EDIT_INTERVAL"] = HIVE_MIN_COMMENT_EDIT_INTERVAL;
  result["HIVE_POST_AVERAGE_WINDOW"] = HIVE_POST_AVERAGE_WINDOW;
  result["HIVE_POST_WEIGHT_CONSTANT"] = HIVE_POST_WEIGHT_CONSTANT;
  result["HIVE_MAX_ACCOUNT_WITNESS_VOTES"] = HIVE_MAX_ACCOUNT_WITNESS_VOTES;
  result["HIVE_DEFAULT_HBD_INTEREST_RATE"] = HIVE_DEFAULT_HBD_INTEREST_RATE;
  result["HIVE_INFLATION_RATE_START_PERCENT"] = HIVE_INFLATION_RATE_START_PERCENT;
  result["HIVE_INFLATION_RATE_STOP_PERCENT"] = HIVE_INFLATION_RATE_STOP_PERCENT;
  result["HIVE_INFLATION_NARROWING_PERIOD"] = HIVE_INFLATION_NARROWING_PERIOD;
  result["HIVE_CONTENT_REWARD_PERCENT_HF16"] = HIVE_CONTENT_REWARD_PERCENT_HF16;
  result["HIVE_VESTING_FUND_PERCENT_HF16"] = HIVE_VESTING_FUND_PERCENT_HF16;
  result["HIVE_PROPOSAL_FUND_PERCENT_HF0"] = HIVE_PROPOSAL_FUND_PERCENT_HF0;
  result["HIVE_CONTENT_REWARD_PERCENT_HF21"] = HIVE_CONTENT_REWARD_PERCENT_HF21;
  result["HIVE_PROPOSAL_FUND_PERCENT_HF21"] = HIVE_PROPOSAL_FUND_PERCENT_HF21;
  result["HIVE_HF21_CONVERGENT_LINEAR_RECENT_CLAIMS"] = HIVE_HF21_CONVERGENT_LINEAR_RECENT_CLAIMS;
  result["HIVE_CONTENT_CONSTANT_HF21"] = HIVE_CONTENT_CONSTANT_HF21;
  result["HIVE_MINER_PAY_PERCENT"] = HIVE_MINER_PAY_PERCENT;
  result["HIVE_MAX_RATION_DECAY_RATE"] = HIVE_MAX_RATION_DECAY_RATE;
  result["HIVE_BANDWIDTH_AVERAGE_WINDOW_SECONDS"] = HIVE_BANDWIDTH_AVERAGE_WINDOW_SECONDS;
  result["HIVE_BANDWIDTH_PRECISION"] = HIVE_BANDWIDTH_PRECISION;
  result["HIVE_MAX_COMMENT_DEPTH_PRE_HF17"] = HIVE_MAX_COMMENT_DEPTH_PRE_HF17;
  result["HIVE_MAX_COMMENT_DEPTH"] = HIVE_MAX_COMMENT_DEPTH;
  result["HIVE_SOFT_MAX_COMMENT_DEPTH"] = HIVE_SOFT_MAX_COMMENT_DEPTH;
  result["HIVE_MAX_RESERVE_RATIO"] = HIVE_MAX_RESERVE_RATIO;
  result["HIVE_CREATE_ACCOUNT_WITH_HIVE_MODIFIER"] = HIVE_CREATE_ACCOUNT_WITH_HIVE_MODIFIER;
  result["HIVE_CREATE_ACCOUNT_DELEGATION_RATIO"] = HIVE_CREATE_ACCOUNT_DELEGATION_RATIO;
  result["HIVE_CREATE_ACCOUNT_DELEGATION_TIME"] = HIVE_CREATE_ACCOUNT_DELEGATION_TIME;
  result["HIVE_MINING_REWARD"] = HIVE_MINING_REWARD;
  result["HIVE_EQUIHASH_N"] = HIVE_EQUIHASH_N;
  result["HIVE_EQUIHASH_K"] = HIVE_EQUIHASH_K;
  result["HIVE_LIQUIDITY_TIMEOUT_SEC"] = HIVE_LIQUIDITY_TIMEOUT_SEC;
  result["HIVE_MIN_LIQUIDITY_REWARD_PERIOD_SEC"] = HIVE_MIN_LIQUIDITY_REWARD_PERIOD_SEC;
  result["HIVE_LIQUIDITY_REWARD_PERIOD_SEC"] = HIVE_LIQUIDITY_REWARD_PERIOD_SEC;
  result["HIVE_LIQUIDITY_REWARD_BLOCKS"] = HIVE_LIQUIDITY_REWARD_BLOCKS;
  result["HIVE_MIN_LIQUIDITY_REWARD"] = HIVE_MIN_LIQUIDITY_REWARD;
  result["HIVE_MIN_CONTENT_REWARD"] = HIVE_MIN_CONTENT_REWARD;
  result["HIVE_MIN_CURATE_REWARD"] = HIVE_MIN_CURATE_REWARD;
  result["HIVE_MIN_PRODUCER_REWARD"] = HIVE_MIN_PRODUCER_REWARD;
  result["HIVE_MIN_POW_REWARD"] = HIVE_MIN_POW_REWARD;
  result["HIVE_ACTIVE_CHALLENGE_FEE"] = HIVE_ACTIVE_CHALLENGE_FEE;
  result["HIVE_OWNER_CHALLENGE_FEE"] = HIVE_OWNER_CHALLENGE_FEE;
  result["HIVE_ACTIVE_CHALLENGE_COOLDOWN"] = HIVE_ACTIVE_CHALLENGE_COOLDOWN;
  result["HIVE_OWNER_CHALLENGE_COOLDOWN"] = HIVE_OWNER_CHALLENGE_COOLDOWN;
  result["HIVE_POST_REWARD_FUND_NAME"] = HIVE_POST_REWARD_FUND_NAME;
  result["HIVE_COMMENT_REWARD_FUND_NAME"] = HIVE_COMMENT_REWARD_FUND_NAME;
  result["HIVE_RECENT_RSHARES_DECAY_TIME_HF17"] = HIVE_RECENT_RSHARES_DECAY_TIME_HF17;
  result["HIVE_RECENT_RSHARES_DECAY_TIME_HF19"] = HIVE_RECENT_RSHARES_DECAY_TIME_HF19;
  result["HIVE_CONTENT_CONSTANT_HF0"] = HIVE_CONTENT_CONSTANT_HF0;
  result["HIVE_APR_PERCENT_MULTIPLY_PER_BLOCK"] = HIVE_APR_PERCENT_MULTIPLY_PER_BLOCK;
  result["HIVE_APR_PERCENT_SHIFT_PER_BLOCK"] = HIVE_APR_PERCENT_SHIFT_PER_BLOCK;
  result["HIVE_APR_PERCENT_MULTIPLY_PER_ROUND"] = HIVE_APR_PERCENT_MULTIPLY_PER_ROUND;
  result["HIVE_APR_PERCENT_SHIFT_PER_ROUND"] = HIVE_APR_PERCENT_SHIFT_PER_ROUND;
  result["HIVE_APR_PERCENT_MULTIPLY_PER_HOUR"] = HIVE_APR_PERCENT_MULTIPLY_PER_HOUR;
  result["HIVE_APR_PERCENT_SHIFT_PER_HOUR"] = HIVE_APR_PERCENT_SHIFT_PER_HOUR;
  result["HIVE_CURATE_APR_PERCENT"] = HIVE_CURATE_APR_PERCENT;
  result["HIVE_CONTENT_APR_PERCENT"] = HIVE_CONTENT_APR_PERCENT;
  result["HIVE_LIQUIDITY_APR_PERCENT"] = HIVE_LIQUIDITY_APR_PERCENT;
  result["HIVE_PRODUCER_APR_PERCENT"] = HIVE_PRODUCER_APR_PERCENT;
  result["HIVE_POW_APR_PERCENT"] = HIVE_POW_APR_PERCENT;
  result["HIVE_MIN_PAYOUT_HBD"] = HIVE_MIN_PAYOUT_HBD;
  result["HIVE_HBD_STOP_PERCENT_HF14"] = HIVE_HBD_STOP_PERCENT_HF14;
  result["HIVE_HBD_STOP_PERCENT_HF20"] = HIVE_HBD_STOP_PERCENT_HF20;
  result["HIVE_HBD_START_PERCENT_HF14"] = HIVE_HBD_START_PERCENT_HF14;
  result["HIVE_HBD_START_PERCENT_HF20"] = HIVE_HBD_START_PERCENT_HF20;
  result["HIVE_MIN_ACCOUNT_NAME_LENGTH"] = HIVE_MIN_ACCOUNT_NAME_LENGTH;
  result["HIVE_MAX_ACCOUNT_NAME_LENGTH"] = HIVE_MAX_ACCOUNT_NAME_LENGTH;
  result["HIVE_MIN_PERMLINK_LENGTH"] = HIVE_MIN_PERMLINK_LENGTH;
  result["HIVE_MAX_PERMLINK_LENGTH"] = HIVE_MAX_PERMLINK_LENGTH;
  result["HIVE_MAX_WITNESS_URL_LENGTH"] = HIVE_MAX_WITNESS_URL_LENGTH;
  result["HIVE_MAX_SHARE_SUPPLY"] = HIVE_MAX_SHARE_SUPPLY;
  result["HIVE_MAX_SATOSHIS"] = HIVE_MAX_SATOSHIS;
  result["HIVE_MAX_SIG_CHECK_DEPTH"] = HIVE_MAX_SIG_CHECK_DEPTH;
  result["HIVE_MAX_SIG_CHECK_ACCOUNTS"] = HIVE_MAX_SIG_CHECK_ACCOUNTS;
  result["HIVE_MIN_TRANSACTION_SIZE_LIMIT"] = HIVE_MIN_TRANSACTION_SIZE_LIMIT;
  result["HIVE_SECONDS_PER_YEAR"] = HIVE_SECONDS_PER_YEAR;
  result["HIVE_HBD_INTEREST_COMPOUND_INTERVAL_SEC"] = HIVE_HBD_INTEREST_COMPOUND_INTERVAL_SEC;
  result["HIVE_MAX_TRANSACTION_SIZE"] = HIVE_MAX_TRANSACTION_SIZE;
  result["HIVE_MIN_BLOCK_SIZE_LIMIT"] = HIVE_MIN_BLOCK_SIZE_LIMIT;
  result["HIVE_MAX_BLOCK_SIZE"] = HIVE_MAX_BLOCK_SIZE;
  result["HIVE_SOFT_MAX_BLOCK_SIZE"] = HIVE_SOFT_MAX_BLOCK_SIZE;
  result["HIVE_MIN_BLOCK_SIZE"] = HIVE_MIN_BLOCK_SIZE;
  result["HIVE_BLOCKS_PER_HOUR"] = HIVE_BLOCKS_PER_HOUR;
  result["HIVE_FEED_INTERVAL_BLOCKS"] = HIVE_FEED_INTERVAL_BLOCKS;
  result["HIVE_FEED_HISTORY_WINDOW_PRE_HF_16"] = HIVE_FEED_HISTORY_WINDOW_PRE_HF_16;
  result["HIVE_FEED_HISTORY_WINDOW"] = HIVE_FEED_HISTORY_WINDOW;
  result["HIVE_MAX_FEED_AGE_SECONDS"] = HIVE_MAX_FEED_AGE_SECONDS;
  result["HIVE_MIN_FEEDS"] = HIVE_MIN_FEEDS;
  result["HIVE_CONVERSION_DELAY_PRE_HF_16"] = HIVE_CONVERSION_DELAY_PRE_HF_16;
  result["HIVE_CONVERSION_DELAY"] = HIVE_CONVERSION_DELAY;
  result["HIVE_COLLATERALIZED_CONVERSION_DELAY"] = HIVE_COLLATERALIZED_CONVERSION_DELAY;
  result["HIVE_CONVERSION_COLLATERAL_RATIO"] = HIVE_CONVERSION_COLLATERAL_RATIO;
  result["HIVE_COLLATERALIZED_CONVERSION_FEE"] = HIVE_COLLATERALIZED_CONVERSION_FEE;
  result["HIVE_MIN_UNDO_HISTORY"] = HIVE_MIN_UNDO_HISTORY;
  result["HIVE_MAX_UNDO_HISTORY"] = HIVE_MAX_UNDO_HISTORY;
  result["HIVE_MIN_TRANSACTION_EXPIRATION_LIMIT"] = HIVE_MIN_TRANSACTION_EXPIRATION_LIMIT;
  result["HIVE_BLOCKCHAIN_PRECISION"] = HIVE_BLOCKCHAIN_PRECISION;
  result["HIVE_BLOCKCHAIN_PRECISION_DIGITS"] = HIVE_BLOCKCHAIN_PRECISION_DIGITS;
  result["HIVE_MAX_INSTANCE_ID"] = HIVE_MAX_INSTANCE_ID;
  result["HIVE_MAX_AUTHORITY_MEMBERSHIP"] = HIVE_MAX_AUTHORITY_MEMBERSHIP;
  result["HIVE_MAX_ASSET_WHITELIST_AUTHORITIES"] = HIVE_MAX_ASSET_WHITELIST_AUTHORITIES;
  result["HIVE_MAX_URL_LENGTH"] = HIVE_MAX_URL_LENGTH;
  result["HIVE_IRREVERSIBLE_THRESHOLD"] = HIVE_IRREVERSIBLE_THRESHOLD;
  result["HIVE_VIRTUAL_SCHEDULE_LAP_LENGTH"] = HIVE_VIRTUAL_SCHEDULE_LAP_LENGTH;
  result["HIVE_VIRTUAL_SCHEDULE_LAP_LENGTH2"] = HIVE_VIRTUAL_SCHEDULE_LAP_LENGTH2;
  result["HIVE_INITIAL_VOTE_POWER_RATE"] = HIVE_INITIAL_VOTE_POWER_RATE;
  result["HIVE_REDUCED_VOTE_POWER_RATE"] = HIVE_REDUCED_VOTE_POWER_RATE;
  result["HIVE_MAX_LIMIT_ORDER_EXPIRATION"] = HIVE_MAX_LIMIT_ORDER_EXPIRATION;
  result["HIVE_DELEGATION_RETURN_PERIOD_HF0"] = HIVE_DELEGATION_RETURN_PERIOD_HF0;
  result["HIVE_DELEGATION_RETURN_PERIOD_HF20"] = HIVE_DELEGATION_RETURN_PERIOD_HF20;
  result["HIVE_RD_MIN_DECAY_BITS"] = HIVE_RD_MIN_DECAY_BITS;
  result["HIVE_RD_MAX_DECAY_BITS"] = HIVE_RD_MAX_DECAY_BITS;
  result["HIVE_RD_DECAY_DENOM_SHIFT"] = HIVE_RD_DECAY_DENOM_SHIFT;
  result["HIVE_RD_MAX_POOL_BITS"] = HIVE_RD_MAX_POOL_BITS;
  result["HIVE_RD_MAX_BUDGET_1"] = HIVE_RD_MAX_BUDGET_1;
  result["HIVE_RD_MAX_BUDGET_2"] = HIVE_RD_MAX_BUDGET_2;
  result["HIVE_RD_MAX_BUDGET_3"] = HIVE_RD_MAX_BUDGET_3;
  result["HIVE_RD_MAX_BUDGET"] = HIVE_RD_MAX_BUDGET;
  result["HIVE_RD_MIN_DECAY"] = HIVE_RD_MIN_DECAY;
  result["HIVE_RD_MIN_BUDGET"] = HIVE_RD_MIN_BUDGET;
  result["HIVE_RD_MAX_DECAY"] = HIVE_RD_MAX_DECAY;
  result["HIVE_ACCOUNT_SUBSIDY_PRECISION"] = HIVE_ACCOUNT_SUBSIDY_PRECISION;
  result["HIVE_WITNESS_SUBSIDY_BUDGET_PERCENT"] = HIVE_WITNESS_SUBSIDY_BUDGET_PERCENT;
  result["HIVE_WITNESS_SUBSIDY_DECAY_PERCENT"] = HIVE_WITNESS_SUBSIDY_DECAY_PERCENT;
  result["HIVE_DEFAULT_ACCOUNT_SUBSIDY_DECAY"] = HIVE_DEFAULT_ACCOUNT_SUBSIDY_DECAY;
  result["HIVE_DEFAULT_ACCOUNT_SUBSIDY_BUDGET"] = HIVE_DEFAULT_ACCOUNT_SUBSIDY_BUDGET;
  result["HIVE_DECAY_BACKSTOP_PERCENT"] = HIVE_DECAY_BACKSTOP_PERCENT;
  result["HIVE_BLOCK_GENERATION_POSTPONED_TX_LIMIT"] = HIVE_BLOCK_GENERATION_POSTPONED_TX_LIMIT;
  result["HIVE_PENDING_TRANSACTION_EXECUTION_LIMIT"] = HIVE_PENDING_TRANSACTION_EXECUTION_LIMIT;
  result["HIVE_CUSTOM_OP_ID_MAX_LENGTH"] = HIVE_CUSTOM_OP_ID_MAX_LENGTH;
  result["HIVE_CUSTOM_OP_DATA_MAX_LENGTH"] = HIVE_CUSTOM_OP_DATA_MAX_LENGTH;
  result["HIVE_BENEFICIARY_LIMIT"] = HIVE_BENEFICIARY_LIMIT;
  result["HIVE_COMMENT_TITLE_LIMIT"] = HIVE_COMMENT_TITLE_LIMIT;
  result["HIVE_ONE_DAY_SECONDS"] = HIVE_ONE_DAY_SECONDS;
  result["HIVE_MINER_ACCOUNT"] = HIVE_MINER_ACCOUNT;
  result["HIVE_NULL_ACCOUNT"] = HIVE_NULL_ACCOUNT;
  result["HIVE_TEMP_ACCOUNT"] = HIVE_TEMP_ACCOUNT;
  result["HIVE_PROXY_TO_SELF_ACCOUNT"] = HIVE_PROXY_TO_SELF_ACCOUNT;
  result["HIVE_ROOT_POST_PARENT"] = HIVE_ROOT_POST_PARENT;
  result["OBSOLETE_TREASURY_ACCOUNT"] = OBSOLETE_TREASURY_ACCOUNT;
  result["NEW_HIVE_TREASURY_ACCOUNT"] = NEW_HIVE_TREASURY_ACCOUNT;
  result["HIVE_TREASURY_FEE"] = HIVE_TREASURY_FEE;
  result["HIVE_PROPOSAL_SUBJECT_MAX_LENGTH"] = HIVE_PROPOSAL_SUBJECT_MAX_LENGTH;
  result["HIVE_PROPOSAL_MAX_IDS_NUMBER"] = HIVE_PROPOSAL_MAX_IDS_NUMBER;
  result["HIVE_PROPOSAL_FEE_INCREASE_DAYS"] = HIVE_PROPOSAL_FEE_INCREASE_DAYS;
  result["HIVE_PROPOSAL_FEE_INCREASE_DAYS_SEC"] = HIVE_PROPOSAL_FEE_INCREASE_DAYS_SEC;
  result["HIVE_PROPOSAL_FEE_INCREASE_AMOUNT"] = HIVE_PROPOSAL_FEE_INCREASE_AMOUNT;
  result["HIVE_PROPOSAL_CONVERSION_RATE"] = HIVE_PROPOSAL_CONVERSION_RATE;

  return result;
}

} } // hive::protocol

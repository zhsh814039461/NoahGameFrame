/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef NFI_NOSQL_MODULE_H
#define NFI_NOSQL_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/ulti/CRC16.h"


typedef std::vector<std::string> string_vector;
typedef std::pair<std::string, std::string> string_pair;
typedef std::vector<string_pair> string_pair_vector;
typedef std::pair<std::string, double> string_score_pair;
typedef std::vector<string_score_pair> string_score_vector;

class NFRedisCommand;
class NFRedisReply;
class NFIRedisClient;
class NFIRedis
{
public:

	virtual bool Connect(const std::string& ip, const int port, const std::string& auth, const bool sync = true) = 0;

	/**
	* @brie if you have setted a password for Redis, you much use AUTH cmd to connect to the server than you can use other cmds
	* @param password
	* @return success: true; fail: false
	*/
	virtual bool AUTH(const std::string& auth) = 0;

	/**
	* @brie select DB
	* @param DB index
	* @return success: true; fail: false
	*/
	//virtual bool SelectDB(int dbnum) = 0;

	/**
	* @brie Get information and statistics about the server
	* @return success: the info
	*/
	virtual bool INFO(std::string& info) = 0;

	/*
	ECHO
	PING
	QUIT
	*/
	/////////client key//////////////
	/**
	* @brie del a key
	* @param key's name
	* @return success: true; fail: false
	*/
	virtual bool DEL(const std::string& key) = 0;

	//NF_SHARE_PTR<NFRedisResult> DUMP(const std::string& key, std::string& out);

	/**
	* @brie Returns if key exists.
	* @param key's name
	* @return true if the key exists, false if the key does not exist.
	*/
	virtual bool EXISTS(const std::string& key) = 0;

	/**
	* @brief Set a timeout on key. After the timeout has expired, the key will automatically be deleted
	* @param keys [in] name of key
	* @param seconds [in] the key will be destroy after this second
	* @return The true for set or reset success, false for failed(if key does not exist)
	*/
	virtual bool EXPIRE(const std::string& key, const unsigned int secs) = 0;

	/**
	* @brief this function like EXPIRE, which is to set the lfie time for one key
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @param timestamp [in] the key will be destroy after this timestamp
	* @return true if the timeout was set, false if key does not exist.
	*/
	virtual bool EXPIREAT(const std::string& key, const int64_t unixTime) = 0;

	/**
	* @brief Remove the existing timeout on key, turning the key from volatile (a key with an expire set) to persistent(a key that will never expire as no timeout is associated).
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @return true if the timeout was removed, false if key does not exist or does not have an associated timeout.
	*/
	virtual bool PERSIST(const std::string& key) = 0;

	/**
	* @brief Returns the remaining time to live of a key that has a timeout
	* @param keys [in] name of key
	* @return TTL in seconds, or a negative value in order to signal an error
	* The command returns -2 if the key does not exist.
	* The command returns -1 if the key exists but has no associated expire.
	*/
	virtual bool TTL(const std::string& key, int& ttl) = 0;

	/**
	* @brief Returns the string representation of the type of the value stored at key
	* @param keys [in] name of key
	* @return Simple string reply: type of key, or none when key does not exist.
	*/
	virtual bool TYPE(const std::string& key, std::string& type) = 0;
	//NF_SHARE_PTR<NFRedisResult> SCAN(const std::string& key);


	/////////client String//////////////
	/**
	* @brief If key already exists and is a string, this command appends the value at the end of the string.
	* If key does not exist it is created and set as an empty string
	* @param keys [in] name of key
	* @param value that you want to append
	* @return the length of the string after the append operation.
	*/
	virtual bool APPEND(const std::string& key, const std::string& value, int& length) = 0;
	//NF_SHARE_PTR<NFRedisResult> BITCOUNT
	//NF_SHARE_PTR<NFRedisResult> BITOP
	//NF_SHARE_PTR<NFRedisResult> BITFIELD(const std::string& key);

	/**
	* @brief Decrements the number stored at key by one.
	* If the key does not exist, it is set to 0 before performing the operation.
	* @param keys [in] name of key
	* @param value that you want to append
	* @return the value of key after the decrement
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
	virtual bool DECR(const std::string& key, int64_t& value) = 0;

	/**
	* @brief Decrements the number stored at key by decrement.
	* If the key does not exist, it is set to 0 before performing the operation.
	* @param keys [in] name of key
	* @param value that you want to append
	* @return the value of key after the decrement
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
	virtual bool DECRBY(const std::string& key, const int64_t decrement, int64_t& value) = 0;

	/**
	* @brief Get the value of key. If the key does not exist the special value "" is returned
	* @param keys [in] name of key
	* @return the value of key, or "" when key does not exist. An error is returned if the value stored at key is not a string
	*/
	virtual bool GET(const std::string& key, std::string & value) = 0;

	//NF_SHARE_PTR<NFRedisResult> GETBIT(const std::string& key);
	//NF_SHARE_PTR<NFRedisResult> GETRANGE(const std::string& key);
	/**
	* @brief Atomically sets key to value and returns the old value stored at key
	* @param keys [in] name of key
	* @return the old value stored at key, or "" when key did not exist.
	*/
	virtual bool GETSET(const std::string& key, const std::string& value, std::string& oldValue) = 0;

	/**
	* @brief Increments the number stored at key by one.
	* If the key does not exist, it is set to 0 before performing the operation
	* @param keys [in] name of key
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
	virtual bool INCR(const std::string& key, int64_t& value) = 0;

	/**
	* @brief Increments the number stored at key by increment
	* If the key does not exist, it is set to 0 before performing the operation
	* @param keys [in] name of key
	* @param keys [in] increment
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
	virtual bool INCRBY(const std::string& key, const int64_t increment, int64_t& value) = 0;

	/**
	* @brief Increment the string representing a floating point number stored at key by the specified increment
	* @param keys [in] name of key
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
	*/
	virtual bool INCRBYFLOAT(const std::string& key, const float increment, float& value) = 0;

	//NF_SHARE_PTR<NFRedisResult> MSETNX(const std::string& key);
	//NF_SHARE_PTR<NFRedisResult> PSETEX(const std::string& key);
	/**
	* @brief Set key to hold the string value. If key already holds a value, it is overwritten, regardless of its type
	* @param key [in] name of key
	* @param value [in] value of the key
	* @return true if SET was executed correctly.
	* false is returned if the SET operation was not performed because the user specified the NX or XX option but the condition was not met.
	*/
	virtual bool SET(const std::string& key, const std::string& value) = 0;

	//NF_SHARE_PTR<NFRedisResult> SETBIT(const std::string& key);
	/**
	* @brief Set key to hold the string value and set key to timeout after a given number of seconds
	* @param key [in] name of key
	* @param value [in] value of the key
	* @param time [in] time that you want set
	* @return true if SETEX was executed correctly, false is returned when seconds is invalid.
	*/
	virtual bool SETEX(const std::string& key, const std::string& value, int time) = 0;

	/**
	* @brief SET if Not eXists --- Set key to hold string value if key does not exist.
	* @param key [in] name of key
	* @param value [in] value of the key
	* @return true if the key was set, false if the key was not set
	*/
	virtual bool SETNX(const std::string& key, const std::string& value) = 0;
	//NF_SHARE_PTR<NFRedisResult> SETRANGE(const std::string& key);

	/**
	* @brief Returns the length of the string value stored at key
	* @param key [in] name of key
	* @param length [out] the length of the string at key
	* @return false when key does not exist or wrong type
	*/
	virtual bool STRLEN(const std::string& key, int& length) = 0;

	/////////client hash//////////////
	/**
	* @brief Removes the specified fields from the hash stored at key
	* @param field/fields [in] the fields you want to remove
	* @return the number of fields that were removed from the hash, not including specified but non existing fields.
	*/
	virtual bool HDEL(const std::string& key, const std::string& field) = 0;
	virtual bool HDEL(const std::string& key, const string_vector& fields) = 0;

	/**
	* @brief Returns if field is an existing field in the hash stored at key.
	* @param field [in] the field you want to check
	* @return true if the hash contains field. false if the hash does not contain field, or key does not exist.
	*/
	virtual bool HEXISTS(const std::string& key, const std::string& field) = 0;

	/**
	* @brief Returns the value associated with field in the hash stored at key.
	* @param field [in] the field you want to get
	* @param value [out] the value you want to get
	* @return true if the hash contains field. false if the hash does not contain field, or key does not exist.
	*/
	virtual bool HGET(const std::string& key, const std::string& field, std::string& value) = 0;

	/**
	* @brief Returns all field names in the hash stored at key.
	* @param key [in] the name of the key
	* @param values [out] all the key & values of the key
	* @return true when key exist, false when key does not exist.
	*/
	virtual bool HGETALL(const std::string& key, std::vector<string_pair>& values) = 0;

	/**
	* @brief Increments the number stored at key by increment
	* If the key does not exist, it is set to 0 before performing the operation
	* @param key [in] name of key
	* @param field [in] field
	* @param by [in] increment
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as integer.
	*/
	virtual bool HINCRBY(const std::string& key, const std::string& field, const int by, int64_t& value) = 0;

	/**
	* @brief Increment the string representing a floating point number stored at key by the specified increment
	* @param key [in] name of key
	* @param field [in] field
	* @param by [in] increment
	* @param value [out] the value of key after the increment
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
	*/
	virtual bool HINCRBYFLOAT(const std::string& key, const std::string& field, const float by, float& value) = 0;

	/**
	* @brief Returns all field names in the hash stored at key.
	* @param key [in] the name of the key
	* @param fields [out] the fields of the key
	* @return true when key exist, false when key does not exist.
	*/
	virtual bool HKEYS(const std::string& key, std::vector<std::string>& fields) = 0;


	/**
	* @brief Returns the number of fields contained in the hash stored at key.
	* @param key [in] the name of the key
	* @param number [out] number of fields in the hash
	* @return true when key exist, false when key does not exist.
	*/
	virtual bool HLEN(const std::string& key, int& number) = 0;

	/**
	* @brief Returns the values associated with the specified fields in the hash stored at key.
	* @param key [in] the name of the key
	* @param field [in] the fields you want to get
	* @param values [out] the values return
	* @return list of values associated with the given fields, in the same order as they are requested.
	*/
	virtual bool HMGET(const std::string& key, const string_vector& fields, string_vector& values) = 0;

	/**
	* @brief Sets the specified fields to their respective values in the hash stored at key
	* @param key [in] the name of the key
	* @param values [in] the fields/value you want to set
	* @return true if cmd success, false when the key not a hashmap
	* If the key does not exist, a new key holding a hash is created
	*/
	virtual bool HMSET(const std::string& key, const std::vector<string_pair>& values) = 0;
	virtual bool HMSET(const std::string& key, const string_vector& fields, const string_vector& values) = 0;

	/**
	* @brief Sets the specified field to their respective values in the hash stored at key
	* @param key [in] the name of the key
	* @param values [in] the fields/value you want to set
	* @return true if cmd success, false when the key not a hashmap
	* If the key does not exist, a new key holding a hash is created
	*/
	virtual bool HSET(const std::string& key, const std::string& field, const std::string& value) = 0;

	/**
	* @brief SET if Not eXists --- Sets field in the hash stored at key to value, only if field does not yet exist
	* @param key [in] name of key
	* @param field [in] field of the hashmap
	* @param value [in] value of the field
	* @return true if the key was set, false if the key was not set(maybe not a hashmap key)
	*/
	virtual bool HSETNX(const std::string& key, const std::string& field, const std::string& value) = 0;

	/**
	* @brief Returns all values in the hash stored at key.
	* @param key [in] name of key
	* @param values [out] fields/values that you want to know
	* @return false when key does not exist or not a hashmap key
	*/
	virtual bool HVALS(const std::string& key, string_vector& values) = 0;
	//NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);

	/**
	* @brief Returns the length of the string value stored at key
	* @param key [in] name of key
	* @param field [in] field that you want to know
	* @param length [out] the length of the string at field, or 0 when field does not exist.
	* @return true when cmd success, false when key does not exist or not a list key.
	*/
	virtual bool HSTRLEN(const std::string& key, const std::string& field, int& length) = 0;

	/////////client list//////////////

	//NF_SHARE_PTR<NFRedisResult> BLPOP(const std::string& key, string_vector& values);
	//NF_SHARE_PTR<NFRedisResult> BRPOP(const std::string& key, string_vector& values);
	//NF_SHARE_PTR<NFRedisResult> BRPOPLPUSH(const std::string& key, string_vector& values);
	/**
	* @brief Returns the element at index index in the list stored at key
	* @param key [in] name of key
	* @param index [in] index that you want to know
	* @param value [out] th value that you got
	* @return true when cmd success, false when key does not exist or not a list key.
	*/
	virtual bool LINDEX(const std::string& key, const int index, std::string& value) = 0;
	//NF_SHARE_PTR<NFRedisResult> LINSERT(const std::string& key, const std::string& value1, const std::string& value2);

	/**
	* @brief Returns the length of the list stored at key
	* @param key [in] name of key
	* @param length [out] the length that you got
	* @return true when cmd success, false when key does not exist or not a list key.
	*/
	virtual bool LLEN(const std::string& key, int& length) = 0;

	/**
	* @brief Removes and returns the first element of the list stored at key.
	* @param key [in] name of key
	* @param value [out] the value of the first element
	* @return true when cmd success, false when key does not exist or not a list key.
	*/
	virtual bool LPOP(const std::string& key, std::string& value) = 0;

	/**
	* @brief Insert all the specified values at the head of the list stored at key
	* @param key [in] name of key
	* @param value [in] the value that you want to push to the head
	* @return length of the list when cmd success, 0 when key does not a list key.
	*/
	virtual bool LPUSH(const std::string& key, const std::string& value, int& length) = 0;

	/**
	* @brief Insert all the specified values at the head of the list stored at key
	* only if key already exists and holds a list.
	* @param key [in] name of key
	* @param value [in] the value that you want to push to the head
	* @return length of the list when cmd success, 0 when key does not a list key.
	*/
	virtual bool LPUSHX(const std::string& key, const std::string& value, int& length) = 0;


	/**
	* @brief Returns the specified elements of the list stored at key by start(included) and end(included)
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @param values [out] the value that you want to get
	* @return true when cmd success, false when key does not exist or dose not a list key.
	*/
	virtual bool LRANGE(const std::string& key, const int start, const int end, string_vector& values) = 0;
	//NF_SHARE_PTR<NFRedisResult> LREM(const std::string& key, string_vector& values);

	/**
	* @brief Sets the list element at index to value
	* @param key [in] name of key
	* @param index [in] the index of this list
	* @param value [in] the value that you want set
	* @return true when cmd success, false when key dose not a list key.
	*/
	virtual bool LSET(const std::string& key, const int index, const std::string& value) = 0;
	//NF_SHARE_PTR<NFRedisResult> LTRIM(const std::string& key, string_vector& values);

	/**
	* @brief Removes and returns the last element of the list stored at key.
	* @param key [in] name of key
	* @param value [out] the value of the last element
	* @return true when cmd success, false when key does not exist or not a list key.
	*/
	virtual bool RPOP(const std::string& key, std::string& value) = 0;
	//NF_SHARE_PTR<NFRedisResult> RPOPLPUSH(const std::string& key, string_vector& values);

	/**
	* @brief Insert all the specified values at the last of the list stored at key
	* @param key [in] name of key
	* @param value [in] the value that you want to push to the head
	* @return length of the list when cmd success, 0 when key does not a list key.
	*/
	virtual bool RPUSH(const std::string& key, const std::string& value, int& length) = 0;

	/**
	* @brief Inserts value at the tail of the list stored at key, only if key already exists and holds a list
	* @param key [in] name of key
	* @param value [in] the value that you want push
	* @return length of the list when cmd success, false when key does not exist or dose not a list key.
	*/
	virtual bool RPUSHX(const std::string& key, const std::string& value, int& length) = 0;

	/////////client set//////////////

	/**
	* @brief cmd SADD
	* @param key [in] name of key
	* @param member [in]
	* @return return the number of elements added to the sets when cmd success.
	*/
	virtual bool SADD(const std::string& key, const std::string& member, int& count) = 0;

	/**
	* @brief cmd SCARD
	* @param key [in] name of key
	* @param count [out] the size of set
	* @return return true when cmd success.
	*/
	virtual bool SCARD(const std::string& key, int& count) = 0;

	/**
	* @brief cmd SDIFF
	* @param key_1 [in] name of diff_key1
	* @param key_2 [in] name of diff_key2
	* @param output [out] difference sets
	* @return return true when cmd success.
	*/
	//virtual bool SDIFF(const std::string& key_1, const std::string& key_2, string_vector& output) = 0;

	/**
	* @brief cmd SDIFFSTORE
	* @param store_key [in] store_diff_key
	* @param key_1 [in] name of diff_key1
	* @param key_2 [in] name of diff_key2
	* @return return true when cmd success.
	*/
	virtual bool SDIFFSTORE(const std::string& store_key, const std::string& diff_key1, const std::string& diff_key2, int& num) = 0;

	/**
	* @brief cmd SINTER
	* @param store_key [in] store_diff_key
	* @param key_1 [in] name of inter_key1
	* @param key_2 [in] name of inter_key2
	* @param output [out] inter_key1 and inter_key2 intersection
	* @return return true when cmd success.
	*/
	//virtual bool SINTER(const std::string& key_1, const std::string& key_2, string_vector& output) = 0;

	/**
	* @brief cmd SINTERSTORE
	* @param store_key [in] inter_store_key
	* @param inter_key1 [in] name of inter_key1
	* @param inter_key2 [in] name of inter_key2
	* @return return true when cmd success.
	*/
	//virtual bool SINTERSTORE(const std::string& inter_store_key, const std::string& inter_key1, const std::string& inter_key2) = 0;

	/**
	* @brief cmd SISMEMBER
	* @param store_key [in] name of key
	* @param member [in] set's member
	* @return return true when the member in set.
	*/
	virtual bool SISMEMBER(const std::string& key, const std::string& member) = 0;

	/**
	* @brief cmd SMEMBERS
	* @param store_key [in] name of key
	* @param output [out] all member of set
	* @return return true when cmd success.
	*/
	virtual bool SMEMBERS(const std::string& key, string_vector& output) = 0;

	/**
	* @brief cmd SMOVE
	* @param source_key [in] name of source set key
	* @param dest_key [in] name of destination set key
	* @param member [in] the member of source set
	* @return return true when cmd success.
	*/
	//virtual bool SMOVE(const std::string& source_key, const std::string& dest_key, const std::string& member) = 0;

	/**
	* @brief cmd SPOP
	* @param key [in] name of key
	* @param output [out] remove member
	* @return return true when cmd success.
	*/
	virtual bool SPOP(const std::string& key, std::string& output) = 0;

	/**
	* @brief cmd SRANDMEMBER
	* @param key [in] name of key
	* @param count [in] the number of member
	* @param output [out] remove members
	* @return return true when cmd success.
	*/
	virtual bool SRANDMEMBER(const std::string& key, int count, string_vector& output) = 0;

	/**
	* @brief cmd SREM
	* @param key [in] name of key
	* @param output [out] remove members
	* @return return the number of remove member
	*/
	virtual bool SREM(const std::string& key, const string_vector& members, int& number) = 0;

	/**
	* @brief cmd SUNION
	* @param union_key1 [in] name of union_key1
	* @param union_key2 [in] name of union_key2
	* @param output [out] set1 and set2 union member
	* @return return true when cmd success.
	*/
	//virtual bool SUNION(const std::string& union_key1, const std::string& union_key2, string_vector& output) = 0;

	/**
	* @brief cmd SUNIONSTORE
	* @param dest_store_key [in] name of destination set
	* @param union_key1 [in] name of union_key1
	* @param union_key2 [out] name of union_key2
	* @return return true when cmd success.
	*/
	virtual bool SUNIONSTORE(const std::string& dest_store_key, const std::string& union_key1, const std::string& union_key2) = 0;


	/////////client SortedSet//////////////
	/**
	* @brief Adds all the specified members with the specified scores to the sorted set stored at key
	* @param key [in] name of key
	* @param member [in]
	* @param score [in]
	* @return return the number of elements added to the sorted sets when cmd success, false when key dose not a z key.
	*/
	virtual bool ZADD(const std::string& key, const std::string& member, const double score, int& number) = 0;

	/**
	* @brief Returns the number of elements of the sorted set stored at key.
	* @param key [in] name of key
	* @return return the number( of elements) of the sorted set, or 0 if key does not exist or not a z key
	*/
	virtual bool ZCARD(const std::string& key, int &count) = 0;

	/**
	* @brief Returns the number of elements in the sorted set at key with a score between min and max.
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @return the number of elements in the specified score range, or 0 if key does not exist or not a z key
	*/
	virtual bool ZCOUNT(const std::string& key, const double start, const double end, int &count) = 0;

	/**
	* @brief Increments the score of member in the sorted set stored at key by increment
	* If member does not exist in the sorted set, it is added with increment as its score (as if its previous score was 0.0)
	* @param key [in] name of key
	* @param member [in]
	* @param score [in]
	* @param newScore [out] the new score of member
	* @return the value of key after the increment
	* An error is returned if the key contains a value of the wrong type or contains a string that can not be represented as float/double.
	*/
	virtual bool ZINCRBY(const std::string& key, const std::string & member, const double score, double& newScore) = 0;

	/**
	* @brief Returns the specified range of elements in the sorted set stored at key
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @param values [out] the members of this range
	* @return true when cmd success, false when key does not exist or not a z key.
	*/
	virtual bool ZRANGE(const std::string& key, const int start, const int end, string_score_vector& values) = 0;

	/**
	* @brief Returns all the elements in the sorted set at key with a score between min and max
	* (including elements with score equal to min or max), the elements are considered to be ordered from low to high scores.
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @param values [out] the members of this range
	* @return true when cmd success, false when key does not exist or not a z key.
	*/
	virtual bool ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values) = 0;

	/**
	* @brief Returns the rank of member in the sorted set stored at key, with the scores ordered from low to high
	* @param key [in] name of key
	* @param member [in] the members of this range
	* @param rank [out] the rank of this member
	* @return true if member exists in the sorted set, false when member does not exist or not a z key.
	*/
	virtual bool ZRANK(const std::string & key, const std::string & member, int& rank) = 0;

	/**
	* @brief Removes the specified members from the sorted set stored at key. Non existing members are ignored.
	* @param key [in] name of key
	* @param member [in] the members of this range
	* @return true if member exists in the sorted set and removed success, false when member does not exist or not a z key.
	*/
	virtual bool ZREM(const std::string& key, const std::string& member) = 0;

	/**
	* @brief Removes all elements in the sorted set stored at key with rank between start and stop
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @return true if cmd removed success, false when the key is not a z key.
	*/
	virtual bool ZREMRANGEBYRANK(const std::string& key, const int start, const int end) = 0;

	/**
	* @brief Removes all elements in the sorted set stored at key with a score between min and max (inclusive).
	* @param key [in] name of key
	* @param min [in]
	* @param max [in]
	* @return true if cmd removed success, false when the key is not a z key.
	*/
	virtual bool ZREMRANGEBYSCORE(const std::string& key, const double min, const double max) = 0;

	/**
	* @brief Returns the specified range of elements in the sorted set stored at key.
	* The elements are considered to be ordered from the highest to the lowest score.
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @param values [out] the members of this range
	* @return true when cmd success, false when key does not exist or not a z key.
	*/
	virtual bool ZREVRANGE(const std::string& key, const int start, const int end, string_score_vector& values) = 0;

	/**
	* @brief Returns all the elements in the sorted set at key with a score between max and min
	* (including elements with score equal to max or min)
	* @param key [in] name of key
	* @param start [in]
	* @param end [in]
	* @param values [out] the members of this range
	* @return true when cmd success, false when key does not exist or not a z key.
	*/
	virtual bool ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values) = 0;


	/**
	* @brief Returns the rank of member in the sorted set stored at key, with the scores ordered from high to low.
	* @param key [in] name of key
	* @param member [in] the members of this range
	* @param rank [out] the rank of this member
	* @return true if member is exists in the sorted set, false when member does not exist or not a z key.
	*/
	virtual bool ZREVRANK(const std::string& key, const std::string& member, int& rank) = 0;

	/**
	* @brief Returns the score of member in the sorted set at key.
	* @param key [in] name of key
	* @param member [in] the members of this range
	* @param rank [out] the rank of this member
	* @return true if member is exists in the sorted set, false when member does not exist or not a z key.
	*/
	virtual bool ZSCORE(const std::string& key, const std::string& member, double& score) = 0;


	/////////client server//////////////
	/**
	* @brief Removes all data of all DB
	*/
	virtual void FLUSHALL() = 0;

	/**
	* @brief Removes all the keys of current DB
	*/
	virtual void FLUSHDB() = 0;


	/////////client pubsub//////////////
	/**	@brief cmd PUBLISH
	* @param key [in] name of key
	* @param value [in] publish's msg
	* @return return true when cmd success.
	*/
	//virtual bool PUBLISH(const std::string& key, const std::string& value) = 0;

	/**	@brief cmd SUBSCRIBE
	* @param key [in] name of key
	* @return return true when cmd success.
	*/
	//virtual bool SUBSCRIBE(const std::string& key) = 0;

	/**	@brief cmd UNSUBSCRIBE
	* @param key [in] name of key
	* @return return true when cmd success.
	*/
	//virtual	bool UNSUBSCRIBE(const std::string& key) = 0;

	//cluster

	/**	@brief cmd CLUSTER NODES
	* @return return true, all of the master nodes of this cluster.
	*/
	virtual	bool CLUSTERNODES(std::vector<std::string>& clusters, bool onlyMasterNode = true, bool includeSelfMaster = false) = 0;

	/**	@brief cmd CLUSTER INFO
	* @return return true, the cluster info of this cluster.
	*/
	virtual	bool CLUSTERINFO(std::string& clusterInfo) = 0;
};

class NFIRedisClient : public NFIRedis
{
public:
	NFIRedisClient(){}
	virtual ~NFIRedisClient() {}


	virtual const std::string& GetAuthKey() { return mstrAuthKey; }

	virtual const std::string& GetIP() { return mstrIP; }

	virtual const int GetPort() { return mnPort; }
	virtual const int StartSlot() { return startSlot; }
	virtual const int EndSlot() { return endSlot; }

	virtual bool Enable() = 0;
	virtual bool Authenticated() = 0;

	virtual bool KeepLive() = 0;
	virtual bool Execute() = 0;

	virtual void UpdateSlot(int start, int end) = 0;

	virtual bool IsConnected() = 0;

	virtual NF_SHARE_PTR<NFRedisReply> BuildSendCmd(const NFRedisCommand& cmd) = 0;
	virtual NF_SHARE_PTR<NFRedisReply> LastReply() = 0;

protected:

	int startSlot = 0;
	int endSlot = 0;

	std::string mstrIP;
	int mnPort;
	std::string mstrAuthKey;
};

class RedisMasterNode
{
public:
	std::string id;
	std::string host;
	int port;
	int slotStart;//included
	int slotEnd;//included
};

class NFINoSqlModule
		: public NFIModule, public NFIRedis
{
public:
	virtual NF_SHARE_PTR<NFIRedisClient> FindRedisClient(int slot) = 0;

	int GetSlotByKey(const std::string &key)
	{
		std::string realKey;
		int pos1 = key.find("{");
		int pos2 = key.find("}");
		if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
		{
			realKey = key.substr(pos1 + 1, pos2 - pos1 - 1);
		}
		else
		{
			realKey = key;
		}

		return CRC16(realKey.c_str(), realKey.length()) & 0x3FFF;// 0x3FFF == 16383
	}

	NF_SHARE_PTR<NFIRedisClient> FindRedisClientByKey(const std::string& key)
	{
		int slot = GetSlotByKey(key);
		if (slot >= 0 && slot <= 16383)
		{
			return FindRedisClient(slot);
		}

		return nullptr;
	}

	virtual bool Enable() = 0;
	virtual void CheckConnect() = 0;
	//如果收到move消息，就调用此函数强制刷新nodes
	virtual void ConnectAllMasterNodes(NF_SHARE_PTR<NFIRedisClient> redisClient) = 0;
};

class NFIAsyncNoSqlModule
		: public NFIModule
{
public:

	virtual void CheckConnect() = 0;

	//如果收到move消息，就调用此函数强制刷新nodes
	virtual void ConnectAllMasterNodes(NF_SHARE_PTR<NFIRedisClient> redisClient) = 0;
};

#endif
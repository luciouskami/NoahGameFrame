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

#include "NFRedisClient.h"

bool NFRedisClient::ZADD(const std::string & key, const std::string & member, const double score, int& number)
{
	NFRedisCommand cmd(GET_NAME(ZADD));
	cmd << key;
	cmd << score;
	cmd << member;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		number = (int)pReply->reply->integer;
		return true;
	}

	return false;
}

bool NFRedisClient::ZCARD(const std::string & key, int &count)
{
	NFRedisCommand cmd(GET_NAME(ZCARD));
	cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		count = (int)pReply->reply->integer;
		return true;
	}

	return false;
}

bool NFRedisClient::ZCOUNT(const std::string & key, const double start, const double end, int &count)
{
	NFRedisCommand cmd(GET_NAME(ZCOUNT));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		count = (int)pReply->reply->integer;
		return true;
	}

	return false;
}

bool NFRedisClient::ZINCRBY(const std::string & key, const std::string & member, const double score, double& newScore)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << score;
	cmd << member;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->reply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->reply->str, newScore);
	}

	return success;
}

bool NFRedisClient::ZRANGE(const std::string & key, const int start, const int end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZRANGE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";
	
	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->reply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->reply->elements; k = k + 2)
			{
				if (pReply->reply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->reply->element[k]->str, pReply->reply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->reply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
			return true;
		}
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool NFRedisClient::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->reply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->reply->elements; k = k + 2)
			{
				if (pReply->reply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->reply->element[k]->str, pReply->reply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->reply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
			return true;
		}
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool NFRedisClient::ZRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->reply->integer;
		return true;
	}

	return false;
}

bool NFRedisClient::ZREM(const std::string & key, const std::string & member)
{
	NFRedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		return (bool)pReply->reply->integer;
	}

	return false;
}

bool NFRedisClient::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		return (bool)pReply->reply->integer;
	}

	return false;
}

bool NFRedisClient::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		return (bool)pReply->reply->integer;
	}

	return false;
}

bool NFRedisClient::ZREVRANGE(const std::string& key, const int start, const int end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->reply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->reply->elements; k = k + 2)
			{
				if (pReply->reply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->reply->element[k]->str, pReply->reply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->reply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
			return true;
		}
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool NFRedisClient::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->reply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->reply->elements; k = k + 2)
			{
				if (pReply->reply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->reply->element[k]->str, pReply->reply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->reply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
			return true;
		}
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool NFRedisClient::ZREVRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->reply->integer;
	}

	return true;
}

bool NFRedisClient::ZSCORE(const std::string & key, const std::string & member, double& score)
{
	NFRedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;


	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->reply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->reply->str, score);
	}

	return success;
}

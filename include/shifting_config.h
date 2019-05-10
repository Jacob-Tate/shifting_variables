//
// Created by night on 5/9/19.
//

#pragma once

struct shifting_config
{
private:
  static const int default_OffsetDelay    = 1000;
  static const int default_OffsetCount    = 1000;

public:
  explicit shifting_config(
      int offsetDelay = default_OffsetDelay,
      int offsetCount = default_OffsetCount,
      bool preformCheck = true,
      bool shuffle = true)
      : offsetDelay(offsetDelay)
      , offsetCount(offsetCount)
      , preformCheck(preformCheck)
      , shuffle(shuffle) {}

  int offsetDelay;
  int offsetCount;
  bool preformCheck;
  bool shuffle;
};

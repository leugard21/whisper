#pragma once

#include <QString>

class SingleInstance {
public:
  explicit SingleInstance(const QString &lockPath);
  ~SingleInstance();

  bool tryLock();
  void unlock();

private:
  class Impl;
  Impl *d;
};

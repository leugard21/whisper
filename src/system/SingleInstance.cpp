#include "SingleInstance.h"

#include <QLockFile>

class SingleInstance::Impl {
public:
  explicit Impl(const QString &path) : lock(path) {
    lock.setStaleLockTime(30000);
  }

  QLockFile lock;
};

SingleInstance::SingleInstance(const QString &path) : d(new Impl(path)) {}

SingleInstance::~SingleInstance() { delete d; }

bool SingleInstance::tryLock() { return d->lock.tryLock(50); }

void SingleInstance::unlock() { d->lock.unlock(); }

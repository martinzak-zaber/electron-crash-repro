#include <napi.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <atomic>

using namespace Napi;

std::atomic_bool threadRunning;
std::thread asyncThread;

std::queue<ThreadSafeFunction> workQueue;
std::mutex queueLock;

void asyncLoop() {
  int counter = 0;

  while (true) {
    ThreadSafeFunction toExec;
    bool hasSomething = false;

    {
      std::lock_guard<std::mutex> guard(queueLock);

      if (!workQueue.empty()) {
        toExec = workQueue.front();
        hasSomething = true;
        workQueue.pop();
      }
    }

    if (hasSomething) {
      counter++;

      toExec.BlockingCall([counter]( Napi::Env env, Function jsCallback) {
        std::string message = "Hello " + std::to_string(counter);
        jsCallback.Call({ String::New(env, message) });
      });

      toExec.Release();
    }

    std::this_thread::sleep_for( std::chrono::milliseconds(100));
  }
}

void MethodAsync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  auto callbackFuncJs = info[0].As<Napi::Function>();

  auto safeFunc = ThreadSafeFunction::New(
    env, callbackFuncJs, "Callback test",
    0, 1);

  std::lock_guard<std::mutex> guard(queueLock);
  workQueue.push(safeFunc);
}

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  if (!threadRunning.exchange(true)) {
    asyncThread = std::thread(asyncLoop);
  }

  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));

  exports.Set(Napi::String::New(env, "helloAsync"),
              Napi::Function::New(env, MethodAsync));
  return exports;
}

NODE_API_MODULE(addon, Init)

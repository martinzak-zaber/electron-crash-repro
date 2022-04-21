Reproduction of the following electron crash.

```
C:\Windows\system32\cmd.exe [39088]: ../../third_party/electron_node/src/api/callback.cc:68: Assertion `(Environment::GetCurrent(isolate)) == (env)' failed.
 1: 00007FF7DD474AA6 node::Buffer::New+50438
 2: 00007FF7DD4747E9 node::Buffer::New+49737
 3: 00007FF7DD416A01 node::CallbackScope::CallbackScope+593
 4: 00007FF7DD4167ED node::CallbackScope::CallbackScope+61
 5: 00007FF7DE02CDB9 node::AsyncResource::CallbackScope::CallbackScope+89
 6: 00007FF7DD9D5DDC node_api_get_module_file_name+4092
 7: 00007FF7DD48802C uv_run+492
 8: 00007FF7DBB8DFB8 node::FreeArrayBufferAllocator+23608
 9: 00007FF7DEFA11E8 cppgc::internal::WriteBarrier::DijkstraMarkingBarrierRangeSlow+3187976
10: 00007FF7DF06A492 Cr_z_crc32+629122
11: 00007FF7DD4A30E2 uv_sleep+93074
12: 00007FF7DD4ACBEA uv_sleep+132762
13: 00007FF7DCEE2C45 uv_timer_get_repeat+6778117
14: 00007FF7DD34F202 IsSandboxedProcess+3875010
15: 00007FF7DBC87ECC v8::metrics::LongTaskStats::LongTaskStats+496028
16: 00007FF7DBC889AD v8::metrics::LongTaskStats::LongTaskStats+498813
17: 00007FF7DBC84F3C v8::metrics::LongTaskStats::LongTaskStats+483852
18: 00007FF7DBC852B0 v8::metrics::LongTaskStats::LongTaskStats+484736
19: 00007FF7DBA27F1D std::__1::__vector_base<v8::CpuProfileDeoptFrame,std::__1::allocator<v8::CpuProfileDeoptFrame> >::__vector_base<v8::CpuProfileDeoptFrame,std::__1::allocator<v8::CpuProfileDeoptFrame> >+46541
20: 00007FF7DF3BD8A2 Cr_z_crc32+4115858
21: 00007FF96F727034 BaseThreadInitThunk+20
22: 00007FF96F8C2651 RtlUserThreadStart+33
```

## Folder structure ##
- node-addon-api - N-API module that exposes asynchronous function
- electron-demo - simple electron application using the module

## Instructions ##
- cd node-addon-api; npm i; cd ..
- cd electron-demo; npm i; npm start

## Reproduction ##
Observe the hello world messages appear.
Click the link and observe messages in the new window.
Click the button and observe failure in the console.

## Notes ##
The entire issue can be prevented by using electron <= 18 and setting `nativeWindowOpen` to `false`.
This forces separate process for the new windows.

Another workaround is to return `deny` in `setWindowOpenHandler` and open the window manually with the provided `url`.
That also forces a separate process.

From my understanding the N-API module is context-aware so it should technically work in child windows.

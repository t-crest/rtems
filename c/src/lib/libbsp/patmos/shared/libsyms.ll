; Start of libsyms.ll
target datalayout = "E-S32-p:32:32:32-i8:8:8-i16:16:16-i32:32:32-i64:32:32-f64:32:32-a0:0:32-s0:32:32-v64:32:32-v128:32:32-n32"
target triple = "patmos-unknown-rtems"

%struct._reent = type opaque

declare i32 @rtems_termios_write(i8* nocapture %arg) nounwind
declare i32 @rtems_io_register_name(i8* nocapture %device_name, i32 %major, i32 %minor) nounwind
declare void @libc_init() nounwind
declare i32 @_kill_r(%struct._reent* nocapture %ptr, i32 %pid, i32 %sig) nounwind
declare i32 @_getpid_r(%struct._reent* nocapture %ptr) nounwind
declare i32 @_exit() nounwind

@llvm.used = appending global [6 x i8*] [
i8* bitcast (i32 (i8*)* @rtems_termios_write to i8*),
i8* bitcast (i32 (i8*, i32, i32)* @rtems_io_register_name to i8*),
i8* bitcast (void ()* @libc_init to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32)* @_kill_r to i8*),
i8* bitcast (i32 (%struct._reent*)* @_getpid_r to i8*),
i8* bitcast (i32 ()* @_exit to i8*)
], section "llvm.metadata"

; End of libsyms.ll
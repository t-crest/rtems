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
declare i32 @_calloc_r(%struct._reent* nocapture %ignored, i32 %elements, i32 %size) nounwind
declare void @_free_r(%struct._reent* nocapture %ignored, i32 %ptr) nounwind
declare i32 @_malloc_r(%struct._reent* nocapture %ignored, i32 %size) nounwind
declare i32 @_realloc_r(%struct._reent* nocapture %ignored, i32 %ptr, i32 %size) nounwind
declare i32 @_read_r(%struct._reent* nocapture %ptr, i32 %fd, i32 %buf, i32 %nbytes) nounwind
declare i32 @_write_r(%struct._reent* nocapture %ptr, i32 %fd, i32 %buf, i32 %nbytes) nounwind
declare i32 @_lseek_r(%struct._reent* nocapture %ptr, i32 %fd, i32 %offset, i32 %whence) nounwind
declare i32 @_close_r(%struct._reent* nocapture %ptr, i32 %fd) nounwind
declare i32 @boot_card(i32 %argc, i8** nocapture %argv, i8** nocapture %envp) nounwind
declare i32 @getdents(i32 %dd_fd, i32 %dd_buf, i32 %dd_len) nounwind
declare i32 @_rename_r(%struct._reent* nocapture %ptr, i32 %old, i32 %new) nounwind
declare i32 @rtems_clock_set_nanoseconds_extension(i32 ()* nocapture %routine) nounwind
declare i32 @rtems_clock_tick() nounwind

@llvm.used = appending global [19 x i8*] [
i8* bitcast (i32 (i8*)* @rtems_termios_write to i8*),
i8* bitcast (i32 (i8*, i32, i32)* @rtems_io_register_name to i8*),
i8* bitcast (void ()* @libc_init to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32)* @_kill_r to i8*),
i8* bitcast (i32 (%struct._reent*)* @_getpid_r to i8*),
i8* bitcast (i32 ()* @_exit to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32)* @_calloc_r to i8*),
i8* bitcast (void (%struct._reent*, i32)* @_free_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32)* @_malloc_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32)* @_realloc_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32, i32)* @_read_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32, i32)* @_write_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32, i32)* @_lseek_r to i8*),
i8* bitcast (i32 (%struct._reent*, i32)* @_close_r to i8*),
i8* bitcast (i32 (i32, i8**, i8**)* @boot_card to i8*),
i8* bitcast (i32 (i32, i32, i32)* @getdents to i8*),
i8* bitcast (i32 (%struct._reent*, i32, i32)* @_rename_r to i8*),
i8* bitcast (i32 (i32()*)* @rtems_clock_set_nanoseconds_extension to i8*),
i8* bitcast (i32 ()* @rtems_clock_tick to i8*)
], section "llvm.metadata"

; End of libsyms.ll
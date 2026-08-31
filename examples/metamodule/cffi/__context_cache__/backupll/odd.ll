; ModuleID = 'odd'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__3oddB2v9B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax = common local_unnamed_addr global ptr null
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x721b4357c2b0>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x721b4357c2b0>\00"
@_ZN08NumbaEnv8test_ffi4evenB2v7B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2ax = common local_unnamed_addr global ptr null

define noundef range(i32 -1, -2) i32 @_ZN8__main__3oddB2v9B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr noalias nocapture writeonly initializes((0, 8)) %retptr, ptr noalias nocapture readnone %excinfo, ptr %arg.ctx, i64 %arg.arg1) local_unnamed_addr {
common.ret:
  %0 = trunc i64 %arg.arg1 to i32
  %1 = mul i32 %0, 3
  %.6.i = add i32 %1, 1
  %.7.i = tail call i32 @_TYPEMAGICNEven6CallFnE(ptr %arg.ctx, i32 %.6.i), !noalias !0
  %.34 = sext i32 %.7.i to i64
  %.35 = add nsw i64 %.34, 1
  store i64 %.35, ptr %retptr, align 8
  ret i32 0
}

define i64 @odd(ptr %.1, i64 %.2) local_unnamed_addr {
entry:
  %.4 = alloca i64, align 8
  %.8 = call i32 @_ZN8__main__3oddB2v9B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr nonnull %.4, ptr nonnull poison, ptr %.1, i64 %.2) #0
  %.18 = load i64, ptr %.4, align 8
  %.20 = alloca i32, align 4
  store i32 0, ptr %.20, align 4
  %cond = icmp eq i32 %.8, 0
  br i1 %cond, label %common.ret, label %entry.if

entry.if:                                         ; preds = %entry
  %.16 = icmp sgt i32 %.8, 0
  call void @numba_gil_ensure(ptr nonnull %.20)
  br i1 %.16, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %.23
  ret i64 %.18

.23:                                              ; preds = %entry.if.endif, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.71 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x721b4357c2b0>")
  call void @PyErr_WriteUnraisable(ptr %.71)
  call void @Py_DecRef(ptr %.71)
  call void @numba_gil_release(ptr nonnull %.20)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  unreachable

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.8, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.23
  ]

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(ptr nonnull @PyExc_StopIteration)
  br label %.23

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(ptr nonnull @PyExc_SystemError, ptr nonnull @".const.unknown error when calling native function")
  br label %.23
}

declare void @numba_gil_ensure(ptr) local_unnamed_addr

declare ptr @PyUnicode_FromString(ptr) local_unnamed_addr

declare void @PyErr_WriteUnraisable(ptr) local_unnamed_addr

declare void @Py_DecRef(ptr) local_unnamed_addr

declare void @numba_gil_release(ptr) local_unnamed_addr

declare void @PyErr_Clear() local_unnamed_addr

declare void @PyErr_SetString(ptr, ptr) local_unnamed_addr

declare void @PyErr_SetNone(ptr) local_unnamed_addr

declare i32 @_TYPEMAGICNEven6CallFnE(ptr, i32) local_unnamed_addr

attributes #0 = { noinline }

!0 = !{!1}
!1 = distinct !{!1, !2, !"_ZN8test_ffi4evenB2v7B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2ax: %retptr"}
!2 = distinct !{!2, !"_ZN8test_ffi4evenB2v7B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2ax"}

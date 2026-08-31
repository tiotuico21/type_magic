; ModuleID = 'add_one'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf = common local_unnamed_addr global ptr null
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x721b4357cd60>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x721b4357cd60>\00"
@_ZN08NumbaEnv8test_ffi6add_itB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2a = common local_unnamed_addr global ptr null

define noundef range(i32 -1, -2) i32 @_ZN8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr noalias nocapture writeonly initializes((0, 4)) %retptr, ptr noalias nocapture readnone %excinfo, ptr %arg.ctx, float %arg.arg1, i8 %arg.arg2, float %arg.arg3) local_unnamed_addr {
B0.endif:
  %.5.i = tail call i32 @_TYPEMAGICNAddIt6CallFnE(ptr %arg.ctx), !noalias !0
  %.10.not = icmp eq i8 %arg.arg2, 0
  %.64 = sitofp i32 %.5.i to double
  %.65 = fpext float %arg.arg3 to double
  %.66 = fadd double %.65, %.64
  %.71 = fptrunc double %.66 to float
  %storemerge = select i1 %.10.not, float %.71, float %arg.arg1
  store float %storemerge, ptr %retptr, align 4
  ret i32 0
}

define float @add_one(ptr %.1, float %.2, i1 %.3, float %.4) local_unnamed_addr {
entry:
  %.6 = alloca float, align 4
  %.10 = zext i1 %.3 to i8
  %.11 = call i32 @_ZN8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr nonnull %.6, ptr nonnull poison, ptr %.1, float %.2, i8 %.10, float %.4) #0
  %.21 = load float, ptr %.6, align 4
  %.23 = alloca i32, align 4
  store i32 0, ptr %.23, align 4
  %cond = icmp eq i32 %.11, 0
  br i1 %cond, label %common.ret, label %entry.if

entry.if:                                         ; preds = %entry
  %.19 = icmp sgt i32 %.11, 0
  call void @numba_gil_ensure(ptr nonnull %.23)
  br i1 %.19, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %.26
  ret float %.21

.26:                                              ; preds = %entry.if.endif, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.74 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x721b4357cd60>")
  call void @PyErr_WriteUnraisable(ptr %.74)
  call void @Py_DecRef(ptr %.74)
  call void @numba_gil_release(ptr nonnull %.23)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  unreachable

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.11, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.26
  ]

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(ptr nonnull @PyExc_StopIteration)
  br label %.26

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(ptr nonnull @PyExc_SystemError, ptr nonnull @".const.unknown error when calling native function")
  br label %.26
}

declare void @numba_gil_ensure(ptr) local_unnamed_addr

declare ptr @PyUnicode_FromString(ptr) local_unnamed_addr

declare void @PyErr_WriteUnraisable(ptr) local_unnamed_addr

declare void @Py_DecRef(ptr) local_unnamed_addr

declare void @numba_gil_release(ptr) local_unnamed_addr

declare void @PyErr_Clear() local_unnamed_addr

declare void @PyErr_SetString(ptr, ptr) local_unnamed_addr

declare void @PyErr_SetNone(ptr) local_unnamed_addr

declare i32 @_TYPEMAGICNAddIt6CallFnE(ptr) local_unnamed_addr

attributes #0 = { noinline }

!0 = !{!1}
!1 = distinct !{!1, !2, !"_ZN8test_ffi6add_itB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2a: %retptr"}
!2 = distinct !{!2, !"_ZN8test_ffi6add_itB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dE7void_2a"}

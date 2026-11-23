; ModuleID = 'add_one'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf = common global ptr null
@PyExc_RuntimeError = external global i8
@".const.Error creating Python tuple from runtime exception arguments" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x7430a3b4fd90>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x7430a3b4fd90>\00"

define i32 @_ZN8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr noalias captures(none) %retptr, ptr noalias captures(none) %excinfo, ptr %arg.ctx, float %arg.arg1, i8 %arg.arg2, float %arg.arg3) {
entry:
  %.10 = icmp eq i8 %arg.arg2, 0
  %.31 = fadd float %arg.arg1, %arg.arg3
  %storemerge = select i1 %.10, float %.31, float %arg.arg1
  store float %storemerge, ptr %retptr, align 4
  ret i32 0
}

define float @cfunc._ZN8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr %.1, float %.2, i1 %.3, float %.4) {
entry:
  %.6 = alloca float, align 4
  store float 0.000000e+00, ptr %.6, align 4
  %excinfo = alloca ptr, align 8
  store ptr null, ptr %excinfo, align 8
  %.10 = zext i1 %.3 to i8
  %.11 = call i32 @_ZN8__main__7add_oneB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr nonnull %.6, ptr nonnull %excinfo, ptr %.1, float %.2, i8 %.10, float %.4) #0
  %.12 = load ptr, ptr %excinfo, align 8
  %.21 = load float, ptr %.6, align 4
  %.23 = alloca i32, align 4
  store i32 0, ptr %.23, align 4
  switch i32 %.11, label %entry.if [
    i32 -2, label %common.ret
    i32 0, label %common.ret
  ], !prof !0

entry.if:                                         ; preds = %entry
  %.19 = icmp sgt i32 %.11, 0
  call void @numba_gil_ensure(ptr nonnull %.23)
  br i1 %.19, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %entry, %.26, %entry.if.if.if.if
  %common.ret.op = phi float [ 0.000000e+00, %entry.if.if.if.if ], [ %.21, %.26 ], [ %.21, %entry ], [ %.21, %entry ]
  ret float %common.ret.op

.26:                                              ; preds = %entry.if.endif, %entry.if.if.endif, %entry.if.if.endif.if, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.74 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x7430a3b4fd90>")
  call void @PyErr_WriteUnraisable(ptr %.74)
  call void @Py_DecRef(ptr %.74)
  call void @numba_gil_release(ptr nonnull %.23)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  %.29 = load { ptr, i32, ptr, ptr, i32 }, ptr %.12, align 8
  %.30 = extractvalue { ptr, i32, ptr, ptr, i32 } %.29, 4
  %.31 = icmp sgt i32 %.30, 0
  %.34 = extractvalue { ptr, i32, ptr, ptr, i32 } %.29, 0
  %.36 = extractvalue { ptr, i32, ptr, ptr, i32 } %.29, 1
  br i1 %.31, label %entry.if.if.if, label %entry.if.if.else

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.11, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.26
  ]

entry.if.if.if:                                   ; preds = %entry.if.if
  %.37 = sext i32 %.36 to i64
  %.38 = call ptr @PyBytes_FromStringAndSize(ptr %.34, i64 %.37)
  %.39 = load { ptr, i32, ptr, ptr, i32 }, ptr %.12, align 8
  %.40 = extractvalue { ptr, i32, ptr, ptr, i32 } %.39, 2
  %.42 = extractvalue { ptr, i32, ptr, ptr, i32 } %.39, 3
  %.44 = call ptr %.42(ptr %.40)
  %.45 = icmp eq ptr %.44, null
  br i1 %.45, label %entry.if.if.if.if, label %entry.if.if.if.endif, !prof !1

entry.if.if.else:                                 ; preds = %entry.if.if
  %.58 = extractvalue { ptr, i32, ptr, ptr, i32 } %.29, 2
  %.59 = call ptr @numba_unpickle(ptr %.34, i32 %.36, ptr %.58)
  br label %entry.if.if.endif

entry.if.if.endif:                                ; preds = %entry.if.if.if.endif, %entry.if.if.else
  %.61 = phi ptr [ %.49, %entry.if.if.if.endif ], [ %.59, %entry.if.if.else ]
  %.62.not = icmp eq ptr %.61, null
  br i1 %.62.not, label %.26, label %entry.if.if.endif.if, !prof !1

entry.if.if.if.if:                                ; preds = %entry.if.if.if
  call void @PyErr_SetString(ptr nonnull @PyExc_RuntimeError, ptr nonnull @".const.Error creating Python tuple from runtime exception arguments")
  br label %common.ret

entry.if.if.if.endif:                             ; preds = %entry.if.if.if
  %.49 = call ptr @numba_runtime_build_excinfo_struct(ptr %.38, ptr nonnull %.44)
  call void @NRT_Free(ptr nonnull %.12)
  br label %entry.if.if.endif

entry.if.if.endif.if:                             ; preds = %entry.if.if.endif
  call void @numba_do_raise(ptr nonnull %.61)
  br label %.26

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(ptr nonnull @PyExc_StopIteration)
  br label %.26

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(ptr nonnull @PyExc_SystemError, ptr nonnull @".const.unknown error when calling native function")
  br label %.26
}

declare void @numba_gil_ensure(ptr)

declare ptr @PyUnicode_FromString(ptr)

declare void @PyErr_WriteUnraisable(ptr)

declare void @Py_DecRef(ptr)

declare void @numba_gil_release(ptr)

declare void @PyErr_Clear()

declare ptr @PyBytes_FromStringAndSize(ptr, i64)

declare ptr @numba_unpickle(ptr, i32, ptr)

declare void @PyErr_SetString(ptr, ptr)

declare ptr @numba_runtime_build_excinfo_struct(ptr, ptr)

declare void @NRT_Free(ptr)

declare void @numba_do_raise(ptr)

declare void @PyErr_SetNone(ptr)

attributes #0 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 49, i32 49}
!1 = !{!"branch_weights", i32 1, i32 99}

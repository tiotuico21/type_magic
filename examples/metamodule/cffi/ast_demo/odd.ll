; ModuleID = 'odd'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__3oddB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax = common global ptr null
@PyExc_RuntimeError = external global i8
@".const.Error creating Python tuple from runtime exception arguments" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x7430a37e6780>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x7430a37e6780>\00"

define i32 @_ZN8__main__3oddB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr noalias captures(none) %retptr, ptr noalias captures(none) %excinfo, ptr %arg.ctx, i64 %arg.arg1) {
B0:
  %.6 = mul nsw i64 %arg.arg1, 3
  %.8 = add nsw i64 %.6, 2
  store i64 %.8, ptr %retptr, align 8
  ret i32 0
}

define i64 @cfunc._ZN8__main__3oddB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr %.1, i64 %.2) {
entry:
  %.4 = alloca i64, align 8
  store i64 0, ptr %.4, align 8
  %excinfo = alloca ptr, align 8
  store ptr null, ptr %excinfo, align 8
  %.8 = call i32 @_ZN8__main__3oddB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr nonnull %.4, ptr nonnull %excinfo, ptr %.1, i64 %.2) #0
  %.9 = load ptr, ptr %excinfo, align 8
  %.18 = load i64, ptr %.4, align 8
  %.20 = alloca i32, align 4
  store i32 0, ptr %.20, align 4
  switch i32 %.8, label %entry.if [
    i32 -2, label %common.ret
    i32 0, label %common.ret
  ], !prof !0

entry.if:                                         ; preds = %entry
  %.16 = icmp sgt i32 %.8, 0
  call void @numba_gil_ensure(ptr nonnull %.20)
  br i1 %.16, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %entry, %.23, %entry.if.if.if.if
  %common.ret.op = phi i64 [ 0, %entry.if.if.if.if ], [ %.18, %.23 ], [ %.18, %entry ], [ %.18, %entry ]
  ret i64 %common.ret.op

.23:                                              ; preds = %entry.if.endif, %entry.if.if.endif, %entry.if.if.endif.if, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.71 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x7430a37e6780>")
  call void @PyErr_WriteUnraisable(ptr %.71)
  call void @Py_DecRef(ptr %.71)
  call void @numba_gil_release(ptr nonnull %.20)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  %.26 = load { ptr, i32, ptr, ptr, i32 }, ptr %.9, align 8
  %.27 = extractvalue { ptr, i32, ptr, ptr, i32 } %.26, 4
  %.28 = icmp sgt i32 %.27, 0
  %.31 = extractvalue { ptr, i32, ptr, ptr, i32 } %.26, 0
  %.33 = extractvalue { ptr, i32, ptr, ptr, i32 } %.26, 1
  br i1 %.28, label %entry.if.if.if, label %entry.if.if.else

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.8, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.23
  ]

entry.if.if.if:                                   ; preds = %entry.if.if
  %.34 = sext i32 %.33 to i64
  %.35 = call ptr @PyBytes_FromStringAndSize(ptr %.31, i64 %.34)
  %.36 = load { ptr, i32, ptr, ptr, i32 }, ptr %.9, align 8
  %.37 = extractvalue { ptr, i32, ptr, ptr, i32 } %.36, 2
  %.39 = extractvalue { ptr, i32, ptr, ptr, i32 } %.36, 3
  %.41 = call ptr %.39(ptr %.37)
  %.42 = icmp eq ptr %.41, null
  br i1 %.42, label %entry.if.if.if.if, label %entry.if.if.if.endif, !prof !1

entry.if.if.else:                                 ; preds = %entry.if.if
  %.55 = extractvalue { ptr, i32, ptr, ptr, i32 } %.26, 2
  %.56 = call ptr @numba_unpickle(ptr %.31, i32 %.33, ptr %.55)
  br label %entry.if.if.endif

entry.if.if.endif:                                ; preds = %entry.if.if.if.endif, %entry.if.if.else
  %.58 = phi ptr [ %.46, %entry.if.if.if.endif ], [ %.56, %entry.if.if.else ]
  %.59.not = icmp eq ptr %.58, null
  br i1 %.59.not, label %.23, label %entry.if.if.endif.if, !prof !1

entry.if.if.if.if:                                ; preds = %entry.if.if.if
  call void @PyErr_SetString(ptr nonnull @PyExc_RuntimeError, ptr nonnull @".const.Error creating Python tuple from runtime exception arguments")
  br label %common.ret

entry.if.if.if.endif:                             ; preds = %entry.if.if.if
  %.46 = call ptr @numba_runtime_build_excinfo_struct(ptr %.35, ptr nonnull %.41)
  call void @NRT_Free(ptr nonnull %.9)
  br label %entry.if.if.endif

entry.if.if.endif.if:                             ; preds = %entry.if.if.endif
  call void @numba_do_raise(ptr nonnull %.58)
  br label %.23

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(ptr nonnull @PyExc_StopIteration)
  br label %.23

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(ptr nonnull @PyExc_SystemError, ptr nonnull @".const.unknown error when calling native function")
  br label %.23
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

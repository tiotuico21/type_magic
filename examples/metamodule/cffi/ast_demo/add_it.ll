; ModuleID = 'add_it'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__6add_itB2v2B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a = common global ptr null
@PyExc_RuntimeError = external global i8
@".const.Error creating Python tuple from runtime exception arguments" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x7430a37e56e0>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x7430a37e56e0>\00"

define i32 @_ZN8__main__6add_itB2v2B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a(ptr noalias captures(none) %retptr, ptr noalias captures(none) %excinfo, ptr %arg.ctx) {
B0:
  store i64 8, ptr %retptr, align 8
  ret i32 0
}

define i64 @cfunc._ZN8__main__6add_itB2v2B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a(ptr %.1) {
entry:
  %.3 = alloca i64, align 8
  store i64 0, ptr %.3, align 8
  %excinfo = alloca ptr, align 8
  store ptr null, ptr %excinfo, align 8
  %.7 = call i32 @_ZN8__main__6add_itB2v2B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a(ptr nonnull %.3, ptr nonnull %excinfo, ptr %.1) #0
  %.8 = load ptr, ptr %excinfo, align 8
  %.17 = load i64, ptr %.3, align 8
  %.19 = alloca i32, align 4
  store i32 0, ptr %.19, align 4
  switch i32 %.7, label %entry.if [
    i32 -2, label %common.ret
    i32 0, label %common.ret
  ], !prof !0

entry.if:                                         ; preds = %entry
  %.15 = icmp sgt i32 %.7, 0
  call void @numba_gil_ensure(ptr nonnull %.19)
  br i1 %.15, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %entry, %.22, %entry.if.if.if.if
  %common.ret.op = phi i64 [ 0, %entry.if.if.if.if ], [ %.17, %.22 ], [ %.17, %entry ], [ %.17, %entry ]
  ret i64 %common.ret.op

.22:                                              ; preds = %entry.if.endif, %entry.if.if.endif, %entry.if.if.endif.if, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.70 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x7430a37e56e0>")
  call void @PyErr_WriteUnraisable(ptr %.70)
  call void @Py_DecRef(ptr %.70)
  call void @numba_gil_release(ptr nonnull %.19)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  %.25 = load { ptr, i32, ptr, ptr, i32 }, ptr %.8, align 8
  %.26 = extractvalue { ptr, i32, ptr, ptr, i32 } %.25, 4
  %.27 = icmp sgt i32 %.26, 0
  %.30 = extractvalue { ptr, i32, ptr, ptr, i32 } %.25, 0
  %.32 = extractvalue { ptr, i32, ptr, ptr, i32 } %.25, 1
  br i1 %.27, label %entry.if.if.if, label %entry.if.if.else

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.7, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.22
  ]

entry.if.if.if:                                   ; preds = %entry.if.if
  %.33 = sext i32 %.32 to i64
  %.34 = call ptr @PyBytes_FromStringAndSize(ptr %.30, i64 %.33)
  %.35 = load { ptr, i32, ptr, ptr, i32 }, ptr %.8, align 8
  %.36 = extractvalue { ptr, i32, ptr, ptr, i32 } %.35, 2
  %.38 = extractvalue { ptr, i32, ptr, ptr, i32 } %.35, 3
  %.40 = call ptr %.38(ptr %.36)
  %.41 = icmp eq ptr %.40, null
  br i1 %.41, label %entry.if.if.if.if, label %entry.if.if.if.endif, !prof !1

entry.if.if.else:                                 ; preds = %entry.if.if
  %.54 = extractvalue { ptr, i32, ptr, ptr, i32 } %.25, 2
  %.55 = call ptr @numba_unpickle(ptr %.30, i32 %.32, ptr %.54)
  br label %entry.if.if.endif

entry.if.if.endif:                                ; preds = %entry.if.if.if.endif, %entry.if.if.else
  %.57 = phi ptr [ %.45, %entry.if.if.if.endif ], [ %.55, %entry.if.if.else ]
  %.58.not = icmp eq ptr %.57, null
  br i1 %.58.not, label %.22, label %entry.if.if.endif.if, !prof !1

entry.if.if.if.if:                                ; preds = %entry.if.if.if
  call void @PyErr_SetString(ptr nonnull @PyExc_RuntimeError, ptr nonnull @".const.Error creating Python tuple from runtime exception arguments")
  br label %common.ret

entry.if.if.if.endif:                             ; preds = %entry.if.if.if
  %.45 = call ptr @numba_runtime_build_excinfo_struct(ptr %.34, ptr nonnull %.40)
  call void @NRT_Free(ptr nonnull %.8)
  br label %entry.if.if.endif

entry.if.if.endif.if:                             ; preds = %entry.if.if.endif
  call void @numba_do_raise(ptr nonnull %.57)
  br label %.22

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(ptr nonnull @PyExc_StopIteration)
  br label %.22

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(ptr nonnull @PyExc_SystemError, ptr nonnull @".const.unknown error when calling native function")
  br label %.22
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

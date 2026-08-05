; ModuleID = 'add_one'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__7add_oneB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2axb = common local_unnamed_addr global ptr null
@".const.Error creating Python tuple from runtime exception arguments" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@".const.<numba.core.cpu.CPUContext object at 0x7c60a0e78e90>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x7c60a0e78e90>\00"
@.const.pickledata.136754459597520 = internal constant [147 x i8] c"\80\04\95\88\00\00\00\00\00\00\00\8C\08builtins\94\8C\0CRuntimeError\94\93\94\8Ccmissing Environment: _ZN08NumbaEnv8__main__8my_printB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEx\94\85\94N\87\94."
@.const.pickledata.136754459597520.sha1 = internal constant [20 x i8] c"-N\0C\F2\97\8C~=\FBU|\A7\9E\1EG\81ux\B9\BA"
@.const.picklebuf.136754459597520 = internal constant { ptr, i32, ptr, ptr, i32 } { ptr @.const.pickledata.136754459597520, i32 147, ptr @.const.pickledata.136754459597520.sha1, ptr null, i32 0 }
@".const.%s" = internal constant [3 x i8] c"%s\00"
@".const.the print() function" = internal constant [21 x i8] c"the print() function\00"
@".const.\0A" = internal constant [2 x i8] c"\0A\00"
@_ZN08NumbaEnv8__main__8my_printB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEx = common local_unnamed_addr global ptr null
@PyExc_RuntimeError = external global i8

define range(i32 0, 2) i32 @_ZN8__main__7add_oneB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2axb(ptr noalias nocapture writeonly %retptr, ptr noalias nocapture writeonly %excinfo, ptr nocapture readnone %arg.ctx, i64 %arg.arg1, i8 %arg.arg2) local_unnamed_addr {
entry:
  %.20 = alloca ptr, align 8
  %excinfo.1 = alloca ptr, align 8
  store ptr null, ptr %excinfo.1, align 8
  %.24 = call i32 @_ZN8__main__8my_printB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEx(ptr nonnull %.20, ptr nonnull %excinfo.1, i64 %arg.arg1)
  %cond = icmp eq i32 %.24, 0
  br i1 %cond, label %B0.endif, label %B0.if.if

common.ret:                                       ; preds = %B0.if.if, %B60, %B40
  %common.ret.op = phi i32 [ 0, %B40 ], [ 0, %B60 ], [ 1, %B0.if.if ]
  ret i32 %common.ret.op

B40:                                              ; preds = %B0.endif
  %.48 = add nsw i64 %arg.arg1, 1
  store i64 %.48, ptr %retptr, align 8
  br label %common.ret

B60:                                              ; preds = %B0.endif
  store i64 0, ptr %retptr, align 8
  br label %common.ret

B0.endif:                                         ; preds = %entry
  %.9.not = icmp eq i8 %arg.arg2, 0
  br i1 %.9.not, label %B60, label %B40

B0.if.if:                                         ; preds = %entry
  %.25 = load ptr, ptr %excinfo.1, align 8
  store ptr %.25, ptr %excinfo, align 8
  br label %common.ret
}

define i64 @add_one(ptr nocapture readnone %.1, i64 %.2, i1 %.3) local_unnamed_addr {
entry:
  %.5 = alloca i64, align 8
  store i64 0, ptr %.5, align 8
  %excinfo = alloca ptr, align 8
  store ptr null, ptr %excinfo, align 8
  %.9 = zext i1 %.3 to i8
  %.10 = call i32 @_ZN8__main__7add_oneB2v4B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2axb(ptr nonnull %.5, ptr nonnull %excinfo, ptr poison, i64 %.2, i8 %.9) #0
  %.11 = load ptr, ptr %excinfo, align 8
  %.20 = load i64, ptr %.5, align 8
  %.22 = alloca i32, align 4
  store i32 0, ptr %.22, align 4
  %cond = icmp eq i32 %.10, 0
  br i1 %cond, label %common.ret, label %entry.if

entry.if:                                         ; preds = %entry
  call void @numba_gil_ensure(ptr nonnull %.22)
  call void @PyErr_Clear()
  %.28 = load { ptr, i32, ptr, ptr, i32 }, ptr %.11, align 8
  %.29 = extractvalue { ptr, i32, ptr, ptr, i32 } %.28, 4
  %.30 = icmp sgt i32 %.29, 0
  %.33 = extractvalue { ptr, i32, ptr, ptr, i32 } %.28, 0
  %.35 = extractvalue { ptr, i32, ptr, ptr, i32 } %.28, 1
  br i1 %.30, label %entry.if.if.if, label %entry.if.if.else

common.ret:                                       ; preds = %entry, %.25, %entry.if.if.if.if
  %common.ret.op = phi i64 [ 0, %entry.if.if.if.if ], [ %.20, %.25 ], [ %.20, %entry ]
  ret i64 %common.ret.op

.25:                                              ; preds = %entry.if.if.endif, %entry.if.if.endif.if
  %.73 = call ptr @PyUnicode_FromString(ptr nonnull @".const.<numba.core.cpu.CPUContext object at 0x7c60a0e78e90>")
  call void @PyErr_WriteUnraisable(ptr %.73)
  call void @Py_DecRef(ptr %.73)
  call void @numba_gil_release(ptr nonnull %.22)
  br label %common.ret

entry.if.if.if:                                   ; preds = %entry.if
  %.36 = sext i32 %.35 to i64
  %.37 = call ptr @PyBytes_FromStringAndSize(ptr %.33, i64 %.36)
  %.38 = load { ptr, i32, ptr, ptr, i32 }, ptr %.11, align 8
  %.39 = extractvalue { ptr, i32, ptr, ptr, i32 } %.38, 2
  %.41 = extractvalue { ptr, i32, ptr, ptr, i32 } %.38, 3
  %.43 = call ptr %.41(ptr %.39)
  %.44 = icmp eq ptr %.43, null
  br i1 %.44, label %entry.if.if.if.if, label %entry.if.if.if.endif, !prof !0

entry.if.if.else:                                 ; preds = %entry.if
  %.57 = extractvalue { ptr, i32, ptr, ptr, i32 } %.28, 2
  %.58 = call ptr @numba_unpickle(ptr %.33, i32 %.35, ptr %.57)
  br label %entry.if.if.endif

entry.if.if.endif:                                ; preds = %entry.if.if.if.endif, %entry.if.if.else
  %.60 = phi ptr [ %.48, %entry.if.if.if.endif ], [ %.58, %entry.if.if.else ]
  %.61.not = icmp eq ptr %.60, null
  br i1 %.61.not, label %.25, label %entry.if.if.endif.if, !prof !0

entry.if.if.if.if:                                ; preds = %entry.if.if.if
  call void @PyErr_SetString(ptr nonnull @PyExc_RuntimeError, ptr nonnull @".const.Error creating Python tuple from runtime exception arguments")
  br label %common.ret

entry.if.if.if.endif:                             ; preds = %entry.if.if.if
  %.48 = call ptr @numba_runtime_build_excinfo_struct(ptr %.37, ptr nonnull %.43)
  call void @NRT_Free(ptr nonnull %.11)
  br label %entry.if.if.endif

entry.if.if.endif.if:                             ; preds = %entry.if.if.endif
  call void @numba_do_raise(ptr nonnull %.60)
  br label %.25
}

declare void @numba_gil_ensure(ptr) local_unnamed_addr

declare ptr @PyUnicode_FromString(ptr) local_unnamed_addr

declare void @PyErr_WriteUnraisable(ptr) local_unnamed_addr

declare void @Py_DecRef(ptr) local_unnamed_addr

declare void @numba_gil_release(ptr) local_unnamed_addr

declare void @PyErr_Clear() local_unnamed_addr

declare ptr @PyBytes_FromStringAndSize(ptr, i64) local_unnamed_addr

declare ptr @numba_unpickle(ptr, i32, ptr) local_unnamed_addr

declare void @PyErr_SetString(ptr, ptr) local_unnamed_addr

declare ptr @numba_runtime_build_excinfo_struct(ptr, ptr) local_unnamed_addr

declare void @NRT_Free(ptr) local_unnamed_addr

declare void @numba_do_raise(ptr) local_unnamed_addr

define linkonce_odr range(i32 0, 2) i32 @_ZN8__main__8my_printB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEx(ptr noalias nocapture writeonly %retptr, ptr noalias nocapture writeonly %excinfo, i64 %arg.value) local_unnamed_addr {
B0:
  %.5 = alloca i32, align 4
  store i32 0, ptr %.5, align 4
  call void @numba_gil_ensure(ptr nonnull %.5)
  %.8 = load ptr, ptr @_ZN08NumbaEnv8__main__8my_printB2v2B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEx, align 8
  %.9 = icmp eq ptr %.8, null
  br i1 %.9, label %B0.if, label %B0.endif, !prof !0

common.ret:                                       ; preds = %B0.endif.endif, %B0.if
  %common.ret.op = phi i32 [ 1, %B0.if ], [ 0, %B0.endif.endif ]
  ret i32 %common.ret.op

B0.if:                                            ; preds = %B0
  store ptr @.const.picklebuf.136754459597520, ptr %excinfo, align 8, !numba_exception_output !1
  br label %common.ret

B0.endif:                                         ; preds = %B0
  %.24 = call ptr @PyLong_FromLongLong(i64 %arg.value)
  %.27.not = icmp eq ptr %.24, null
  br i1 %.27.not, label %B0.endif.else, label %B0.endif.if, !prof !0

B0.endif.if:                                      ; preds = %B0.endif
  %.29 = call ptr @PyObject_Str(ptr nonnull %.24)
  %.30 = call ptr @PyUnicode_AsUTF8(ptr %.29)
  call void (ptr, ...) @PySys_FormatStdout(ptr nonnull @".const.%s", ptr %.30)
  call void @Py_DecRef(ptr %.29)
  br label %B0.endif.endif

B0.endif.else:                                    ; preds = %B0.endif
  %.35 = call ptr @PyUnicode_FromString(ptr nonnull @".const.the print() function")
  call void @PyErr_WriteUnraisable(ptr %.35)
  br label %B0.endif.endif

B0.endif.endif:                                   ; preds = %B0.endif.else, %B0.endif.if
  %.35.sink = phi ptr [ %.35, %B0.endif.else ], [ %.24, %B0.endif.if ]
  call void @Py_DecRef(ptr %.35.sink)
  call void (ptr, ...) @PySys_FormatStdout(ptr nonnull @".const.\0A")
  call void @numba_gil_release(ptr nonnull %.5)
  store ptr null, ptr %retptr, align 8
  br label %common.ret
}

declare ptr @PyLong_FromLongLong(i64) local_unnamed_addr

declare ptr @PyObject_Str(ptr) local_unnamed_addr

declare ptr @PyUnicode_AsUTF8(ptr) local_unnamed_addr

declare void @PySys_FormatStdout(ptr, ...) local_unnamed_addr

attributes #0 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 99}
!1 = !{i1 true}

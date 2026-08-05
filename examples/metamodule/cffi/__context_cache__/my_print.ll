; ModuleID = 'my_print'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.const.pickledata.126763814650368 = internal constant [147 x i8] c"\80\04\95\88\00\00\00\00\00\00\00\8C\08builtins\94\8C\0CRuntimeError\94\93\94\8Ccmissing Environment: _ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx\94\85\94N\87\94."
@.const.pickledata.126763814650368.sha1 = internal constant [20 x i8] c"\AE\C8\0A*}\9F\80\82P\C2\08D;\F2-\EB\F6h\C8\8F"
@.const.picklebuf.126763814650368 = internal constant { i8*, i32, i8*, i8*, i32 } { i8* getelementptr inbounds ([147 x i8], [147 x i8]* @.const.pickledata.126763814650368, i32 0, i32 0), i32 147, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.const.pickledata.126763814650368.sha1, i32 0, i32 0), i8* null, i32 0 }
@".const.%s" = internal constant [3 x i8] c"%s\00"
@".const.the print() function" = internal constant [21 x i8] c"the print() function\00"
@".const.\0A" = internal constant [2 x i8] c"\0A\00"
@.const.my_print = internal constant [9 x i8] c"my_print\00"
@_ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx = common local_unnamed_addr global i8* null
@".const.missing Environment: _ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx" = internal constant [100 x i8] c"missing Environment: _ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx\00"
@".const.Error creating Python tuple from runtime exception arguments" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@".const.unknown error when calling native function" = internal constant [43 x i8] c"unknown error when calling native function\00"
@PyExc_RuntimeError = external global i8
@".const.Error creating Python tuple from runtime exception arguments.1" = internal constant [61 x i8] c"Error creating Python tuple from runtime exception arguments\00"
@PyExc_StopIteration = external global i8
@PyExc_SystemError = external global i8
@".const.unknown error when calling native function.2" = internal constant [43 x i8] c"unknown error when calling native function\00"
@".const.<numba.core.cpu.CPUContext object at 0x734a80259a00>" = internal constant [53 x i8] c"<numba.core.cpu.CPUContext object at 0x734a80259a00>\00"

define i32 @my_print(i64* noalias nocapture writeonly %retptr, { i8*, i32, i8*, i8*, i32 }** noalias nocapture writeonly %excinfo, i64 %arg.value) local_unnamed_addr {
entry:
  %.5 = alloca i32, align 4
  store i32 0, i32* %.5, align 4
  call void @numba_gil_ensure(i32* nonnull %.5)
  %.8 = load i8*, i8** @_ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx, align 8
  %.9 = icmp eq i8* %.8, null
  br i1 %.9, label %B0.if, label %B0.endif, !prof !0

common.ret:                                       ; preds = %B0.endif.endif, %B0.if
  %common.ret.op = phi i32 [ 1, %B0.if ], [ 0, %B0.endif.endif ]
  ret i32 %common.ret.op

B0.if:                                            ; preds = %entry
  store { i8*, i32, i8*, i8*, i32 }* @.const.picklebuf.126763814650368, { i8*, i32, i8*, i8*, i32 }** %excinfo, align 8, !numba_exception_output !1
  br label %common.ret

B0.endif:                                         ; preds = %entry
  %.24 = call i8* @PyLong_FromLongLong(i64 %arg.value)
  %.27.not = icmp eq i8* %.24, null
  br i1 %.27.not, label %B0.endif.else, label %B0.endif.if, !prof !0

B0.endif.if:                                      ; preds = %B0.endif
  %.29 = call i8* @PyObject_Str(i8* nonnull %.24)
  %.30 = call i8* @PyUnicode_AsUTF8(i8* %.29)
  call void (i8*, ...) @PySys_FormatStdout(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @".const.%s", i64 0, i64 0), i8* %.30)
  call void @Py_DecRef(i8* %.29)
  call void @Py_DecRef(i8* nonnull %.24)
  br label %B0.endif.endif

B0.endif.else:                                    ; preds = %B0.endif
  %.35 = call i8* @PyUnicode_FromString(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @".const.the print() function", i64 0, i64 0))
  call void @PyErr_WriteUnraisable(i8* %.35)
  call void @Py_DecRef(i8* %.35)
  br label %B0.endif.endif

B0.endif.endif:                                   ; preds = %B0.endif.else, %B0.endif.if
  call void (i8*, ...) @PySys_FormatStdout(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @".const.\0A", i64 0, i64 0))
  call void @numba_gil_release(i32* nonnull %.5)
  store i64 0, i64* %retptr, align 8
  br label %common.ret
}

declare void @numba_gil_ensure(i32*) local_unnamed_addr

declare i8* @PyLong_FromLongLong(i64) local_unnamed_addr

declare i8* @PyObject_Str(i8*) local_unnamed_addr

declare i8* @PyUnicode_AsUTF8(i8*) local_unnamed_addr

declare void @PySys_FormatStdout(i8*, ...) local_unnamed_addr

declare void @Py_DecRef(i8*) local_unnamed_addr

declare i8* @PyUnicode_FromString(i8*) local_unnamed_addr

declare void @PyErr_WriteUnraisable(i8*) local_unnamed_addr

declare void @numba_gil_release(i32*) local_unnamed_addr

define i8* @_ZN7cpython8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx(i8* nocapture readnone %py_closure, i8* %py_args, i8* nocapture readnone %py_kws) local_unnamed_addr {
entry:
  %.5 = alloca i8*, align 8
  %.6 = call i32 (i8*, i8*, i64, i64, ...) @PyArg_UnpackTuple(i8* %py_args, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.const.my_print, i64 0, i64 0), i64 1, i64 1, i8** nonnull %.5)
  %.7 = icmp eq i32 %.6, 0
  %.36 = alloca i64, align 8
  %excinfo = alloca { i8*, i32, i8*, i8*, i32 }*, align 8
  store { i8*, i32, i8*, i8*, i32 }* null, { i8*, i32, i8*, i8*, i32 }** %excinfo, align 8
  br i1 %.7, label %common.ret, label %entry.endif, !prof !0

common.ret:                                       ; preds = %entry.endif.endif.endif.endif.endif.if.endif, %entry.endif.endif.endif.endif.endif.if.endif.if, %entry.endif.endif.endif.endif.endif.endif.endif.endif, %entry.endif.endif.endif, %entry, %entry.endif.endif.endif.endif.endif.if.if.if, %entry.endif.endif.endif.endif.if.endif, %entry.endif.if
  %common.ret.op = phi i8* [ null, %entry.endif.if ], [ %.57, %entry.endif.endif.endif.endif.if.endif ], [ null, %entry.endif.endif.endif.endif.endif.if.if.if ], [ null, %entry ], [ null, %entry.endif.endif.endif ], [ null, %entry.endif.endif.endif.endif.endif.endif.endif.endif ], [ null, %entry.endif.endif.endif.endif.endif.if.endif.if ], [ null, %entry.endif.endif.endif.endif.endif.if.endif ]
  ret i8* %common.ret.op

entry.endif:                                      ; preds = %entry
  %.11 = load i8*, i8** @_ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx, align 8
  %.16 = icmp eq i8* %.11, null
  br i1 %.16, label %entry.endif.if, label %entry.endif.endif, !prof !0

entry.endif.if:                                   ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([100 x i8], [100 x i8]* @".const.missing Environment: _ZN08NumbaEnv8__main__8my_printB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEx", i64 0, i64 0))
  br label %common.ret

entry.endif.endif:                                ; preds = %entry.endif
  %.20 = load i8*, i8** %.5, align 8
  %.23 = call i8* @PyNumber_Long(i8* %.20)
  %.24.not = icmp eq i8* %.23, null
  br i1 %.24.not, label %entry.endif.endif.endif, label %entry.endif.endif.if, !prof !0

entry.endif.endif.if:                             ; preds = %entry.endif.endif
  %.26 = call i64 @PyLong_AsLongLong(i8* nonnull %.23)
  call void @Py_DecRef(i8* nonnull %.23)
  br label %entry.endif.endif.endif

entry.endif.endif.endif:                          ; preds = %entry.endif.endif.if, %entry.endif.endif
  %.21.0 = phi i64 [ %.26, %entry.endif.endif.if ], [ 0, %entry.endif.endif ]
  %.31 = call i8* @PyErr_Occurred()
  %.32.not = icmp eq i8* %.31, null
  br i1 %.32.not, label %entry.endif.endif.endif.endif, label %common.ret, !prof !2

entry.endif.endif.endif.endif:                    ; preds = %entry.endif.endif.endif
  store i64 0, i64* %.36, align 8
  %.40 = call i32 @my_print(i64* nonnull %.36, { i8*, i32, i8*, i8*, i32 }** nonnull %excinfo, i64 %.21.0) #0
  %.41 = load { i8*, i32, i8*, i8*, i32 }*, { i8*, i32, i8*, i8*, i32 }** %excinfo, align 8
  %.48 = icmp sgt i32 %.40, 0
  %.49 = select i1 %.48, { i8*, i32, i8*, i8*, i32 }* %.41, { i8*, i32, i8*, i8*, i32 }* undef
  switch i32 %.40, label %entry.endif.endif.endif.endif.endif [
    i32 -2, label %entry.endif.endif.endif.endif.if.endif
    i32 0, label %entry.endif.endif.endif.endif.if.endif
  ]

entry.endif.endif.endif.endif.endif:              ; preds = %entry.endif.endif.endif.endif
  %0 = icmp sgt i32 %.40, 0
  br i1 %0, label %entry.endif.endif.endif.endif.endif.if, label %entry.endif.endif.endif.endif.endif.endif.endif.endif

entry.endif.endif.endif.endif.if.endif:           ; preds = %entry.endif.endif.endif.endif, %entry.endif.endif.endif.endif
  %.50 = load i64, i64* %.36, align 8
  %.57 = call i8* @PyLong_FromLongLong(i64 %.50)
  br label %common.ret

entry.endif.endif.endif.endif.endif.if:           ; preds = %entry.endif.endif.endif.endif.endif
  call void @PyErr_Clear()
  %.64 = load { i8*, i32, i8*, i8*, i32 }, { i8*, i32, i8*, i8*, i32 }* %.49, align 8
  %.65 = extractvalue { i8*, i32, i8*, i8*, i32 } %.64, 4
  %.66 = icmp sgt i32 %.65, 0
  %.69 = extractvalue { i8*, i32, i8*, i8*, i32 } %.64, 0
  %.71 = extractvalue { i8*, i32, i8*, i8*, i32 } %.64, 1
  br i1 %.66, label %entry.endif.endif.endif.endif.endif.if.if, label %entry.endif.endif.endif.endif.endif.if.else

entry.endif.endif.endif.endif.endif.if.if:        ; preds = %entry.endif.endif.endif.endif.endif.if
  %.72 = sext i32 %.71 to i64
  %.73 = call i8* @PyBytes_FromStringAndSize(i8* %.69, i64 %.72)
  %.74 = load { i8*, i32, i8*, i8*, i32 }, { i8*, i32, i8*, i8*, i32 }* %.49, align 8
  %.75 = extractvalue { i8*, i32, i8*, i8*, i32 } %.74, 2
  %.77 = extractvalue { i8*, i32, i8*, i8*, i32 } %.74, 3
  %.78 = bitcast i8* %.77 to i8* (i8*)*
  %.79 = call i8* %.78(i8* %.75)
  %.80 = icmp eq i8* %.79, null
  br i1 %.80, label %entry.endif.endif.endif.endif.endif.if.if.if, label %entry.endif.endif.endif.endif.endif.if.if.endif, !prof !0

entry.endif.endif.endif.endif.endif.if.else:      ; preds = %entry.endif.endif.endif.endif.endif.if
  %.93 = extractvalue { i8*, i32, i8*, i8*, i32 } %.64, 2
  %.94 = call i8* @numba_unpickle(i8* %.69, i32 %.71, i8* %.93)
  br label %entry.endif.endif.endif.endif.endif.if.endif

entry.endif.endif.endif.endif.endif.if.endif:     ; preds = %entry.endif.endif.endif.endif.endif.if.if.endif, %entry.endif.endif.endif.endif.endif.if.else
  %.96 = phi i8* [ %.84, %entry.endif.endif.endif.endif.endif.if.if.endif ], [ %.94, %entry.endif.endif.endif.endif.endif.if.else ]
  %.97.not = icmp eq i8* %.96, null
  br i1 %.97.not, label %common.ret, label %entry.endif.endif.endif.endif.endif.if.endif.if, !prof !0

entry.endif.endif.endif.endif.endif.if.if.if:     ; preds = %entry.endif.endif.endif.endif.endif.if.if
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([61 x i8], [61 x i8]* @".const.Error creating Python tuple from runtime exception arguments", i64 0, i64 0))
  br label %common.ret

entry.endif.endif.endif.endif.endif.if.if.endif:  ; preds = %entry.endif.endif.endif.endif.endif.if.if
  %.84 = call i8* @numba_runtime_build_excinfo_struct(i8* %.73, i8* nonnull %.79)
  %.85 = bitcast { i8*, i32, i8*, i8*, i32 }* %.49 to i8*
  call void @NRT_Free(i8* nonnull %.85)
  br label %entry.endif.endif.endif.endif.endif.if.endif

entry.endif.endif.endif.endif.endif.if.endif.if:  ; preds = %entry.endif.endif.endif.endif.endif.if.endif
  call void @numba_do_raise(i8* nonnull %.96)
  br label %common.ret

entry.endif.endif.endif.endif.endif.endif.endif.endif: ; preds = %entry.endif.endif.endif.endif.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_SystemError, i8* getelementptr inbounds ([43 x i8], [43 x i8]* @".const.unknown error when calling native function", i64 0, i64 0))
  br label %common.ret
}

declare i32 @PyArg_UnpackTuple(i8*, i8*, i64, i64, ...) local_unnamed_addr

declare void @PyErr_SetString(i8*, i8*) local_unnamed_addr

declare i8* @PyNumber_Long(i8*) local_unnamed_addr

declare i64 @PyLong_AsLongLong(i8*) local_unnamed_addr

declare i8* @PyErr_Occurred() local_unnamed_addr

declare void @PyErr_Clear() local_unnamed_addr

declare i8* @PyBytes_FromStringAndSize(i8*, i64) local_unnamed_addr

declare i8* @numba_unpickle(i8*, i32, i8*) local_unnamed_addr

declare i8* @numba_runtime_build_excinfo_struct(i8*, i8*) local_unnamed_addr

declare void @NRT_Free(i8*) local_unnamed_addr

declare void @numba_do_raise(i8*) local_unnamed_addr

declare void @PyErr_SetNone(i8*) local_unnamed_addr

define i64 @cfunc.my_print(i64 %.1) local_unnamed_addr {
entry:
  %.3 = alloca i64, align 8
  store i64 0, i64* %.3, align 8
  %excinfo = alloca { i8*, i32, i8*, i8*, i32 }*, align 8
  store { i8*, i32, i8*, i8*, i32 }* null, { i8*, i32, i8*, i8*, i32 }** %excinfo, align 8
  %.7 = call i32 @my_print(i64* nonnull %.3, { i8*, i32, i8*, i8*, i32 }** nonnull %excinfo, i64 %.1) #0
  %.8 = load { i8*, i32, i8*, i8*, i32 }*, { i8*, i32, i8*, i8*, i32 }** %excinfo, align 8
  %.9.not = icmp eq i32 %.7, 0
  %.15 = icmp sgt i32 %.7, 0
  %.16 = select i1 %.15, { i8*, i32, i8*, i8*, i32 }* %.8, { i8*, i32, i8*, i8*, i32 }* undef
  %.17 = load i64, i64* %.3, align 8
  %.19 = alloca i32, align 4
  store i32 0, i32* %.19, align 4
  br i1 %.9.not, label %common.ret, label %entry.if, !prof !2

entry.if:                                         ; preds = %entry
  %0 = icmp sgt i32 %.7, 0
  call void @numba_gil_ensure(i32* nonnull %.19)
  br i1 %0, label %entry.if.if, label %entry.if.endif

common.ret:                                       ; preds = %entry, %.22, %entry.if.if.if.if
  %common.ret.op = phi i64 [ 0, %entry.if.if.if.if ], [ %.17, %.22 ], [ %.17, %entry ]
  ret i64 %common.ret.op

.22:                                              ; preds = %entry.if.endif, %entry.if.if.endif, %entry.if.if.endif.if, %entry.if.endif.endif.endif, %entry.if.endif.if
  %.70 = call i8* @PyUnicode_FromString(i8* getelementptr inbounds ([53 x i8], [53 x i8]* @".const.<numba.core.cpu.CPUContext object at 0x734a80259a00>", i64 0, i64 0))
  call void @PyErr_WriteUnraisable(i8* %.70)
  call void @Py_DecRef(i8* %.70)
  call void @numba_gil_release(i32* nonnull %.19)
  br label %common.ret

entry.if.if:                                      ; preds = %entry.if
  call void @PyErr_Clear()
  %.25 = load { i8*, i32, i8*, i8*, i32 }, { i8*, i32, i8*, i8*, i32 }* %.16, align 8
  %.26 = extractvalue { i8*, i32, i8*, i8*, i32 } %.25, 4
  %.27 = icmp sgt i32 %.26, 0
  %.30 = extractvalue { i8*, i32, i8*, i8*, i32 } %.25, 0
  %.32 = extractvalue { i8*, i32, i8*, i8*, i32 } %.25, 1
  br i1 %.27, label %entry.if.if.if, label %entry.if.if.else

entry.if.endif:                                   ; preds = %entry.if
  switch i32 %.7, label %entry.if.endif.endif.endif [
    i32 -3, label %entry.if.endif.if
    i32 -1, label %.22
  ]

entry.if.if.if:                                   ; preds = %entry.if.if
  %.33 = sext i32 %.32 to i64
  %.34 = call i8* @PyBytes_FromStringAndSize(i8* %.30, i64 %.33)
  %.35 = load { i8*, i32, i8*, i8*, i32 }, { i8*, i32, i8*, i8*, i32 }* %.16, align 8
  %.36 = extractvalue { i8*, i32, i8*, i8*, i32 } %.35, 2
  %.38 = extractvalue { i8*, i32, i8*, i8*, i32 } %.35, 3
  %.39 = bitcast i8* %.38 to i8* (i8*)*
  %.40 = call i8* %.39(i8* %.36)
  %.41 = icmp eq i8* %.40, null
  br i1 %.41, label %entry.if.if.if.if, label %entry.if.if.if.endif, !prof !0

entry.if.if.else:                                 ; preds = %entry.if.if
  %.54 = extractvalue { i8*, i32, i8*, i8*, i32 } %.25, 2
  %.55 = call i8* @numba_unpickle(i8* %.30, i32 %.32, i8* %.54)
  br label %entry.if.if.endif

entry.if.if.endif:                                ; preds = %entry.if.if.if.endif, %entry.if.if.else
  %.57 = phi i8* [ %.45, %entry.if.if.if.endif ], [ %.55, %entry.if.if.else ]
  %.58.not = icmp eq i8* %.57, null
  br i1 %.58.not, label %.22, label %entry.if.if.endif.if, !prof !0

entry.if.if.if.if:                                ; preds = %entry.if.if.if
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([61 x i8], [61 x i8]* @".const.Error creating Python tuple from runtime exception arguments.1", i64 0, i64 0))
  br label %common.ret

entry.if.if.if.endif:                             ; preds = %entry.if.if.if
  %.45 = call i8* @numba_runtime_build_excinfo_struct(i8* %.34, i8* nonnull %.40)
  %.46 = bitcast { i8*, i32, i8*, i8*, i32 }* %.16 to i8*
  call void @NRT_Free(i8* nonnull %.46)
  br label %entry.if.if.endif

entry.if.if.endif.if:                             ; preds = %entry.if.if.endif
  call void @numba_do_raise(i8* nonnull %.57)
  br label %.22

entry.if.endif.if:                                ; preds = %entry.if.endif
  call void @PyErr_SetNone(i8* nonnull @PyExc_StopIteration)
  br label %.22

entry.if.endif.endif.endif:                       ; preds = %entry.if.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_SystemError, i8* getelementptr inbounds ([43 x i8], [43 x i8]* @".const.unknown error when calling native function.2", i64 0, i64 0))
  br label %.22
}

attributes #0 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 99}
!1 = !{i1 true}
!2 = !{!"branch_weights", i32 99, i32 1}

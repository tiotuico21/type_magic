; ModuleID = 'func'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.const.func = internal constant [5 x i8] c"func\00"
@_ZN08NumbaEnv8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd = common local_unnamed_addr global i8* null
@".const.missing Environment: _ZN08NumbaEnv8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd" = internal constant [97 x i8] c"missing Environment: _ZN08NumbaEnv8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd\00"
@PyExc_RuntimeError = external global i8

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i32 @_ZN8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd(double* noalias nocapture writeonly %retptr, { i8*, i32, i8*, i8*, i32 }** noalias nocapture readnone %excinfo, double %arg.x, double %arg.y) local_unnamed_addr #0 {
entry:
  %.6 = fadd double %arg.x, %arg.y
  store double %.6, double* %retptr, align 8
  ret i32 0
}

define i8* @_ZN7cpython8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd(i8* nocapture readnone %py_closure, i8* %py_args, i8* nocapture readnone %py_kws) local_unnamed_addr {
entry:
  %.5 = alloca i8*, align 8
  %.6 = alloca i8*, align 8
  %.7 = call i32 (i8*, i8*, i64, i64, ...) @PyArg_UnpackTuple(i8* %py_args, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.const.func, i64 0, i64 0), i64 2, i64 2, i8** nonnull %.5, i8** nonnull %.6)
  %.8 = icmp eq i32 %.7, 0
  %.39 = alloca double, align 8
  br i1 %.8, label %common.ret, label %entry.endif, !prof !0

common.ret:                                       ; preds = %entry.endif.endif.endif, %entry.endif.endif, %entry, %entry.endif.endif.endif.endif, %entry.endif.if
  %common.ret.op = phi i8* [ null, %entry.endif.if ], [ %.58, %entry.endif.endif.endif.endif ], [ null, %entry ], [ null, %entry.endif.endif ], [ null, %entry.endif.endif.endif ]
  ret i8* %common.ret.op

entry.endif:                                      ; preds = %entry
  %.12 = load i8*, i8** @_ZN08NumbaEnv8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd, align 8
  %.17 = icmp eq i8* %.12, null
  br i1 %.17, label %entry.endif.if, label %entry.endif.endif, !prof !0

entry.endif.if:                                   ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([97 x i8], [97 x i8]* @".const.missing Environment: _ZN08NumbaEnv8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd", i64 0, i64 0))
  br label %common.ret

entry.endif.endif:                                ; preds = %entry.endif
  %.21 = load i8*, i8** %.5, align 8
  %.22 = call i8* @PyNumber_Float(i8* %.21)
  %.23 = call double @PyFloat_AsDouble(i8* %.22)
  call void @Py_DecRef(i8* %.22)
  %.25 = call i8* @PyErr_Occurred()
  %.26.not = icmp eq i8* %.25, null
  br i1 %.26.not, label %entry.endif.endif.endif, label %common.ret, !prof !1

entry.endif.endif.endif:                          ; preds = %entry.endif.endif
  %.30 = load i8*, i8** %.6, align 8
  %.31 = call i8* @PyNumber_Float(i8* %.30)
  %.32 = call double @PyFloat_AsDouble(i8* %.31)
  call void @Py_DecRef(i8* %.31)
  %.34 = call i8* @PyErr_Occurred()
  %.35.not = icmp eq i8* %.34, null
  br i1 %.35.not, label %entry.endif.endif.endif.endif, label %common.ret, !prof !1

entry.endif.endif.endif.endif:                    ; preds = %entry.endif.endif.endif
  store double 0.000000e+00, double* %.39, align 8
  %.43 = call i32 @_ZN8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd(double* nonnull %.39, { i8*, i32, i8*, i8*, i32 }** nonnull undef, double %.23, double %.32) #1
  %.53 = load double, double* %.39, align 8
  %.58 = call i8* @PyFloat_FromDouble(double %.53)
  br label %common.ret
}

declare i32 @PyArg_UnpackTuple(i8*, i8*, i64, i64, ...) local_unnamed_addr

declare void @PyErr_SetString(i8*, i8*) local_unnamed_addr

declare i8* @PyNumber_Float(i8*) local_unnamed_addr

declare double @PyFloat_AsDouble(i8*) local_unnamed_addr

declare void @Py_DecRef(i8*) local_unnamed_addr

declare i8* @PyErr_Occurred() local_unnamed_addr

declare i8* @PyFloat_FromDouble(double) local_unnamed_addr

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define double @cfunc._ZN8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd(double %.1, double %.2) local_unnamed_addr #0 {
entry:
  %.4 = alloca double, align 8
  store double 0.000000e+00, double* %.4, align 8
  %.8 = call i32 @_ZN8__main__4funcB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dEdd(double* nonnull %.4, { i8*, i32, i8*, i8*, i32 }** nonnull undef, double %.1, double %.2) #1
  %.18 = load double, double* %.4, align 8
  ret double %.18
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn writeonly }
attributes #1 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 99}
!1 = !{!"branch_weights", i32 99, i32 1}

; ModuleID = 'add_it'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.const.add_it = internal constant [7 x i8] c"add_it\00"
@_ZN08NumbaEnv8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a = common local_unnamed_addr global i8* null
@".const.missing Environment: _ZN08NumbaEnv8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a" = internal constant [105 x i8] c"missing Environment: _ZN08NumbaEnv8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a\00"
@PyExc_TypeError = external global i8
@".const.can't unbox void* type" = internal constant [23 x i8] c"can't unbox void* type\00"
@PyExc_RuntimeError = external global i8

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i32 @add_it(i64* noalias nocapture writeonly %retptr, { i8*, i32, i8*, i8*, i32 }** noalias nocapture readnone %excinfo, i8* nocapture readnone %arg.ctx) local_unnamed_addr #0 {
entry:
  store i64 8, i64* %retptr, align 8
  ret i32 0
}

define noalias i8* @_ZN7cpython8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a(i8* nocapture readnone %py_closure, i8* %py_args, i8* nocapture readnone %py_kws) local_unnamed_addr {
entry:
  %.5 = alloca i8*, align 8
  %.6 = call i32 (i8*, i8*, i64, i64, ...) @PyArg_UnpackTuple(i8* %py_args, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.const.add_it, i64 0, i64 0), i64 1, i64 1, i8** nonnull %.5)
  %.7 = icmp eq i32 %.6, 0
  br i1 %.7, label %common.ret, label %entry.endif, !prof !0

common.ret:                                       ; preds = %entry, %entry.endif.if, %entry.endif.endif
  ret i8* null

entry.endif:                                      ; preds = %entry
  %.11 = load i8*, i8** @_ZN08NumbaEnv8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a, align 8
  %.16 = icmp eq i8* %.11, null
  br i1 %.16, label %entry.endif.if, label %entry.endif.endif, !prof !0

entry.endif.if:                                   ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([105 x i8], [105 x i8]* @".const.missing Environment: _ZN08NumbaEnv8__main__6add_itB2v4B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2a", i64 0, i64 0))
  br label %common.ret

entry.endif.endif:                                ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_TypeError, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @".const.can't unbox void* type", i64 0, i64 0))
  br label %common.ret
}

declare i32 @PyArg_UnpackTuple(i8*, i8*, i64, i64, ...) local_unnamed_addr

declare void @PyErr_SetString(i8*, i8*) local_unnamed_addr

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i64 @cfunc.add_it(i8* nocapture readnone %.1) local_unnamed_addr #0 {
entry:
  %.3 = alloca i64, align 8
  store i64 0, i64* %.3, align 8
  %.7 = call i32 @add_it(i64* nonnull %.3, { i8*, i32, i8*, i8*, i32 }** nonnull undef, i8* undef) #1
  %.17 = load i64, i64* %.3, align 8
  ret i64 %.17
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn writeonly }
attributes #1 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 99}

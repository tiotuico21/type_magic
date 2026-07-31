; ModuleID = 'add_one'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.const.add_one = internal constant [8 x i8] c"add_one\00"
@_ZN08NumbaEnv8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb = common local_unnamed_addr global i8* null
@".const.missing Environment: _ZN08NumbaEnv8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb" = internal constant [108 x i8] c"missing Environment: _ZN08NumbaEnv8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb\00"
@PyExc_TypeError = external global i8
@".const.can't unbox void* type" = internal constant [23 x i8] c"can't unbox void* type\00"
@PyExc_RuntimeError = external global i8

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i32 @add_one(i64* noalias nocapture writeonly %retptr, { i8*, i32, i8*, i8*, i32 }** noalias nocapture readnone %excinfo, i8* nocapture readnone %arg.ctx, i64 %arg.arg1, i8 %arg.arg2) local_unnamed_addr #0 {
entry:
  %.9 = icmp eq i8 %arg.arg2, 0
  %.20 = add nsw i64 %arg.arg1, 1
  %storemerge = select i1 %.9, i64 0, i64 %.20
  store i64 %storemerge, i64* %retptr, align 8
  ret i32 0
}

define noalias i8* @_ZN7cpython8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb(i8* nocapture readnone %py_closure, i8* %py_args, i8* nocapture readnone %py_kws) local_unnamed_addr {
entry:
  %.5 = alloca i8*, align 8
  %.6 = alloca i8*, align 8
  %.7 = alloca i8*, align 8
  %.8 = call i32 (i8*, i8*, i64, i64, ...) @PyArg_UnpackTuple(i8* %py_args, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.const.add_one, i64 0, i64 0), i64 3, i64 3, i8** nonnull %.5, i8** nonnull %.6, i8** nonnull %.7)
  %.9 = icmp eq i32 %.8, 0
  br i1 %.9, label %common.ret, label %entry.endif, !prof !0

common.ret:                                       ; preds = %entry, %entry.endif.if, %entry.endif.endif
  ret i8* null

entry.endif:                                      ; preds = %entry
  %.13 = load i8*, i8** @_ZN08NumbaEnv8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb, align 8
  %.18 = icmp eq i8* %.13, null
  br i1 %.18, label %entry.endif.if, label %entry.endif.endif, !prof !0

entry.endif.if:                                   ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_RuntimeError, i8* getelementptr inbounds ([108 x i8], [108 x i8]* @".const.missing Environment: _ZN08NumbaEnv8__main__7add_oneB2v3B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb", i64 0, i64 0))
  br label %common.ret

entry.endif.endif:                                ; preds = %entry.endif
  call void @PyErr_SetString(i8* nonnull @PyExc_TypeError, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @".const.can't unbox void* type", i64 0, i64 0))
  br label %common.ret
}

declare i32 @PyArg_UnpackTuple(i8*, i8*, i64, i64, ...) local_unnamed_addr

declare void @PyErr_SetString(i8*, i8*) local_unnamed_addr

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i64 @cfunc.add_one(i8* nocapture readnone %.1, i64 %.2, i1 %.3) local_unnamed_addr #0 {
entry:
  %.5 = alloca i64, align 8
  store i64 0, i64* %.5, align 8
  %.9 = zext i1 %.3 to i8
  %.10 = call i32 @add_one(i64* nonnull %.5, { i8*, i32, i8*, i8*, i32 }** nonnull undef, i8* undef, i64 %.2, i8 %.9) #1
  %.20 = load i64, i64* %.5, align 8
  ret i64 %.20
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn writeonly }
attributes #1 = { noinline }

!0 = !{!"branch_weights", i32 1, i32 99}

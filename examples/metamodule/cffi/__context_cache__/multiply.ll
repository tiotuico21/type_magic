; ModuleID = 'multiply'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd = common local_unnamed_addr global i8* null

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define i32 @_ZN8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd(double* noalias nocapture writeonly %retptr, { i8*, i32, i8*, i8*, i32 }** noalias nocapture readnone %excinfo, double %arg.x, double %arg.y) local_unnamed_addr #0 {
entry:
  %.6 = fmul double %arg.x, %arg.y
  store double %.6, double* %retptr, align 8
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly
define double @cfunc._ZN8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd(double %.1, double %.2) local_unnamed_addr #0 {
entry:
  %.4 = alloca double, align 8
  store double 0.000000e+00, double* %.4, align 8
  %.8 = call i32 @_ZN8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd(double* nonnull %.4, { i8*, i32, i8*, i8*, i32 }** nonnull undef, double %.1, double %.2) #1
  %.18 = load double, double* %.4, align 8
  ret double %.18
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn writeonly }
attributes #1 = { noinline }

; ModuleID = 'sub_args'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__8sub_argsB2v7B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf = common local_unnamed_addr global ptr null

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define noundef i32 @_ZN8__main__8sub_argsB2v7B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr noalias nocapture writeonly initializes((0, 4)) %retptr, ptr noalias nocapture readnone %excinfo, ptr nocapture readnone %arg.ctx, float %arg.arg1, i8 %arg.arg2, float %arg.arg3) local_unnamed_addr #0 {
entry:
  %.10 = icmp eq i8 %arg.arg2, 0
  %.31 = fsub float %arg.arg1, %arg.arg3
  %storemerge = select i1 %.10, float %.31, float %arg.arg1
  store float %storemerge, ptr %retptr, align 4
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define float @sub_args(ptr nocapture readnone %.1, float %.2, i1 %.3, float %.4) local_unnamed_addr #0 {
entry:
  %.6 = alloca float, align 4
  %.10 = zext i1 %.3 to i8
  %.11 = call i32 @_ZN8__main__8sub_argsB2v7B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2afbf(ptr nonnull %.6, ptr nonnull poison, ptr poison, float %.2, i8 %.10, float %.4) #1
  %.21 = load float, ptr %.6, align 4
  ret float %.21
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { noinline }

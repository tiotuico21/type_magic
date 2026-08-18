; ModuleID = 'is_true'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__7is_trueB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ab = common local_unnamed_addr global ptr null

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define noundef i32 @_ZN8__main__7is_trueB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ab(ptr noalias nocapture writeonly initializes((0, 1)) %retptr, ptr noalias nocapture readnone %excinfo, ptr nocapture readnone %arg.ctx, i8 %arg.arg1) local_unnamed_addr #0 {
entry:
  %.8 = icmp ne i8 %arg.arg1, 0
  %.15 = zext i1 %.8 to i8
  store i8 %.15, ptr %retptr, align 1
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define i1 @is_true(ptr nocapture readnone %.1, i1 %.2) local_unnamed_addr #0 {
entry:
  %.4 = alloca i8, align 1
  %.8 = zext i1 %.2 to i8
  %.9 = call i32 @_ZN8__main__7is_trueB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ab(ptr nonnull %.4, ptr nonnull poison, ptr poison, i8 %.8) #1
  %.19 = load i8, ptr %.4, align 1
  %.22 = icmp ne i8 %.19, 0
  ret i1 %.22
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { noinline }

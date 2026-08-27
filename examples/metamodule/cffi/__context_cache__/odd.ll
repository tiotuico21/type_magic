; ModuleID = 'odd'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__3oddB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax = common local_unnamed_addr global ptr null

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define noundef i32 @_ZN8__main__3oddB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr noalias nocapture writeonly initializes((0, 8)) %retptr, ptr noalias nocapture readnone %excinfo, ptr nocapture readnone %arg.ctx, i64 %arg.arg1) local_unnamed_addr #0 {
B0:
  store i64 1, ptr %retptr, align 8
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define i64 @odd(ptr nocapture readnone %.1, i64 %.2) local_unnamed_addr #0 {
entry:
  %.4 = alloca i64, align 8
  %.8 = call i32 @_ZN8__main__3oddB2v8B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2ax(ptr nonnull %.4, ptr nonnull poison, ptr poison, i64 poison) #1
  %.18 = load i64, ptr %.4, align 8
  ret i64 %.18
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { noinline }

; ModuleID = 'add_it'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZN08NumbaEnv8__main__6add_itB2v5B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a = common local_unnamed_addr global ptr null

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define noundef i32 @_ZN8__main__6add_itB2v5B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a(ptr noalias nocapture writeonly initializes((0, 8)) %retptr, ptr noalias nocapture readnone %excinfo, ptr nocapture readnone %arg.ctx) local_unnamed_addr #0 {
B0:
  store i64 8, ptr %retptr, align 8
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define i64 @add_it(ptr nocapture readnone %.1) local_unnamed_addr #0 {
entry:
  %.3 = alloca i64, align 8
  %.7 = call i32 @_ZN8__main__6add_itB2v5B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE7void_2a(ptr nonnull %.3, ptr nonnull poison, ptr poison) #1
  %.17 = load i64, ptr %.3, align 8
  ret i64 %.17
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { noinline }

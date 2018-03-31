diff --git a/arch/arm64/include/asm/futex.h b/arch/arm64/include/asm/futex.h
index 85c4a8981d47..1943cf623366 100644
--- a/arch/arm64/include/asm/futex.h
+++ b/arch/arm64/include/asm/futex.h
@@ -48,13 +48,14 @@ do {									\
 } while (0)
 
 static inline int
-futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
+futex_atomic_op_inuser (int encoded_op, u32 __user *_uaddr)
 {
 	int op = (encoded_op >> 28) & 7;
 	int cmp = (encoded_op >> 24) & 15;
 	int oparg = (encoded_op << 8) >> 20;
 	int cmparg = (encoded_op << 20) >> 20;
 	int oldval = 0, ret, tmp;
+	u32 __user *uaddr = __uaccess_mask_ptr(_uaddr);
 
 	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
 		oparg = 1 << oparg;
@@ -106,15 +107,17 @@ futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
 }
 
 static inline int
-futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
+futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *_uaddr,
 			      u32 oldval, u32 newval)
 {
 	int ret = 0;
 	u32 val, tmp;
+	u32 __user *uaddr;
 
-	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
+	if (!access_ok(VERIFY_WRITE, _uaddr, sizeof(u32)))
 		return -EFAULT;
 
+	uaddr = __uaccess_mask_ptr(_uaddr);
 	uaccess_enable();
 	asm volatile("// futex_atomic_cmpxchg_inatomic\n"
 "	prfm	pstl1strm, %2\n"

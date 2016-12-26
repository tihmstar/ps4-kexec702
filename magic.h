#ifdef PS4_3_55

#define kern_off_printf 0x1df550
#define kern_off_copyin 0x3b96e0
#define kern_off_copyout 0x3b9660
#define kern_off_copyinstr 0x3b9a50
#define kern_off_kmem_alloc_contig 0x337ea0
#define kern_off_kmem_free 0x33bca0
#define kern_off_pmap_extract 0x3afd70
#define kern_off_pmap_protect 0x3b1f50
#define kern_off_sched_pin 0x1ced60
#define kern_off_sched_unpin 0x1cedc0
#define kern_off_smp_rendezvous 0x1e7810
#define kern_off_smp_no_rendevous_barrier 0x1e75d0
#define kern_off_icc_query_nowait 0x3ed450
#define kern_off_kernel_map 0x196acc8
#define kern_off_sysent 0xeed880
#define kern_off_kernel_pmap_store 0x19bd628
#define kern_off_Starsha_UcodeInfo 0x1869fa0

#define kern_off_pml4pml4i 0x19bd618
#define kern_off_dmpml4i 0x19bd61c
#define kern_off_dmpdpi 0x19bd620

#elif defined PS4_3_70

#define kern_off_printf 0x1df620
#define kern_off_copyin 0x3b97d0
#define kern_off_copyout 0x3b9750
#define kern_off_copyinstr 0x3b9b40
#define kern_off_kmem_alloc_contig 0x337f70
#define kern_off_kmem_free 0x33bd70
#define kern_off_pmap_extract 0x3afe60
#define kern_off_pmap_protect 0x3b2040
#define kern_off_sched_pin 0x1cee30
#define kern_off_sched_unpin 0x1cee90
#define kern_off_smp_rendezvous 0x1e78e0
#define kern_off_smp_no_rendevous_barrier 0x1e76a0
#define kern_off_icc_query_nowait 0x3ed7f0
#define kern_off_kernel_map 0x1976cc8
#define kern_off_sysent 0xef6d90
#define kern_off_kernel_pmap_store 0x19c9628
// ps4 Starsha_UcodeInfo (devid!=0x9920)
#define kern_off_Starsha_UcodeInfo 0x18734b0
// ps4pro Starsha_UcodeInfo_GL_Bx (devid==0x9924 and gpu reg 0x203e reads as 0x10)
//#define kern_off_Starsha_UcodeInfo 0x18b1f50

#define kern_off_pml4pml4i 0x19c9618
#define kern_off_dmpml4i 0x19c961c
#define kern_off_dmpdpi 0x19c9620

#elif defined PS4_4_00 || PS4_4_01

#define kern_off_printf 0x347450
#define kern_off_copyin 0x286cc0
#define kern_off_copyout 0x286c40
#define kern_off_copyinstr 0x287030
#define kern_off_kmem_alloc_contig 0x275da0
#define kern_off_kmem_free 0x369580
#define kern_off_pmap_extract 0x3eeed0
#define kern_off_pmap_protect 0x3f1120
#define kern_off_sched_pin 0x1d1120
#define kern_off_sched_unpin 0x1d1180
#define kern_off_smp_rendezvous 0x34a020
#define kern_off_smp_no_rendevous_barrier 0x349de0
#define kern_off_icc_query_nowait 0x46c5a0
#define kern_off_kernel_map 0x1fe71b8
#define kern_off_sysent 0xf17790
#define kern_off_kernel_pmap_store 0x200c310
#define kern_off_Starsha_UcodeInfo 0x18dafb0

#define kern_off_pml4pml4i 0x200c300
#define kern_off_dmpml4i 0x200c304
#define kern_off_dmpdpi 0x200c308

#elif defined PS4_4_05

#define kern_off_printf 0x347580
#define kern_off_copyin 0x286df0
#define kern_off_copyout 0x286d70
#define kern_off_copyinstr 0x287160
#define kern_off_kmem_alloc_contig 0x275ed0
#define kern_off_kmem_free 0x3696b0
#define kern_off_pmap_extract 0x3ef000
#define kern_off_pmap_protect 0x3f1250
#define kern_off_sched_pin 0x1d1250
#define kern_off_sched_unpin 0x1d12B0
#define kern_off_smp_rendezvous 0x34a150
#define kern_off_smp_no_rendevous_barrier 0x349f10
#define kern_off_icc_query_nowait 0x46c6d0
#define kern_off_kernel_map 0x1fe71b8
#define kern_off_sysent 0xf17790
#define kern_off_kernel_pmap_store 0x200c310
#define kern_off_Starsha_UcodeInfo 0x18dafb0

#define kern_off_pml4pml4i 0x200c300
#define kern_off_dmpml4i 0x200c304
#define kern_off_dmpdpi 0x200c308

#endif

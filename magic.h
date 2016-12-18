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

#endif

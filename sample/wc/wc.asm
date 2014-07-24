
wc:     file format elf64-x86-64


Disassembly of section .init:

0000000000400470 <_init>:
  400470:	48 83 ec 08          	sub    $0x8,%rsp
  400474:	e8 a3 00 00 00       	callq  40051c <call_gmon_start>
  400479:	e8 32 01 00 00       	callq  4005b0 <frame_dummy>
  40047e:	e8 fd 03 00 00       	callq  400880 <__do_global_ctors_aux>
  400483:	48 83 c4 08          	add    $0x8,%rsp
  400487:	c3                   	retq   

Disassembly of section .plt:

0000000000400490 <fclose@plt-0x10>:
  400490:	ff 35 5a 0b 20 00    	pushq  0x200b5a(%rip)        # 600ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
  400496:	ff 25 5c 0b 20 00    	jmpq   *0x200b5c(%rip)        # 600ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
  40049c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004004a0 <fclose@plt>:
  4004a0:	ff 25 5a 0b 20 00    	jmpq   *0x200b5a(%rip)        # 601000 <_GLOBAL_OFFSET_TABLE_+0x18>
  4004a6:	68 00 00 00 00       	pushq  $0x0
  4004ab:	e9 e0 ff ff ff       	jmpq   400490 <_init+0x20>

00000000004004b0 <printf@plt>:
  4004b0:	ff 25 52 0b 20 00    	jmpq   *0x200b52(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x20>
  4004b6:	68 01 00 00 00       	pushq  $0x1
  4004bb:	e9 d0 ff ff ff       	jmpq   400490 <_init+0x20>

00000000004004c0 <fgetc@plt>:
  4004c0:	ff 25 4a 0b 20 00    	jmpq   *0x200b4a(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x28>
  4004c6:	68 02 00 00 00       	pushq  $0x2
  4004cb:	e9 c0 ff ff ff       	jmpq   400490 <_init+0x20>

00000000004004d0 <__libc_start_main@plt>:
  4004d0:	ff 25 42 0b 20 00    	jmpq   *0x200b42(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x30>
  4004d6:	68 03 00 00 00       	pushq  $0x3
  4004db:	e9 b0 ff ff ff       	jmpq   400490 <_init+0x20>

00000000004004e0 <fopen@plt>:
  4004e0:	ff 25 3a 0b 20 00    	jmpq   *0x200b3a(%rip)        # 601020 <_GLOBAL_OFFSET_TABLE_+0x38>
  4004e6:	68 04 00 00 00       	pushq  $0x4
  4004eb:	e9 a0 ff ff ff       	jmpq   400490 <_init+0x20>

Disassembly of section .text:

00000000004004f0 <_start>:
  4004f0:	31 ed                	xor    %ebp,%ebp
  4004f2:	49 89 d1             	mov    %rdx,%r9
  4004f5:	5e                   	pop    %rsi
  4004f6:	48 89 e2             	mov    %rsp,%rdx
  4004f9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4004fd:	50                   	push   %rax
  4004fe:	54                   	push   %rsp
  4004ff:	49 c7 c0 70 08 40 00 	mov    $0x400870,%r8
  400506:	48 c7 c1 e0 07 40 00 	mov    $0x4007e0,%rcx
  40050d:	48 c7 c7 3e 06 40 00 	mov    $0x40063e,%rdi
  400514:	e8 b7 ff ff ff       	callq  4004d0 <__libc_start_main@plt>
  400519:	f4                   	hlt    
  40051a:	90                   	nop
  40051b:	90                   	nop

000000000040051c <call_gmon_start>:
  40051c:	48 83 ec 08          	sub    $0x8,%rsp
  400520:	48 8b 05 b9 0a 20 00 	mov    0x200ab9(%rip),%rax        # 600fe0 <_DYNAMIC+0x190>
  400527:	48 85 c0             	test   %rax,%rax
  40052a:	74 02                	je     40052e <call_gmon_start+0x12>
  40052c:	ff d0                	callq  *%rax
  40052e:	48 83 c4 08          	add    $0x8,%rsp
  400532:	c3                   	retq   
  400533:	90                   	nop
  400534:	90                   	nop
  400535:	90                   	nop
  400536:	90                   	nop
  400537:	90                   	nop
  400538:	90                   	nop
  400539:	90                   	nop
  40053a:	90                   	nop
  40053b:	90                   	nop
  40053c:	90                   	nop
  40053d:	90                   	nop
  40053e:	90                   	nop
  40053f:	90                   	nop

0000000000400540 <__do_global_dtors_aux>:
  400540:	55                   	push   %rbp
  400541:	48 89 e5             	mov    %rsp,%rbp
  400544:	53                   	push   %rbx
  400545:	48 83 ec 08          	sub    $0x8,%rsp
  400549:	80 3d e8 0a 20 00 00 	cmpb   $0x0,0x200ae8(%rip)        # 601038 <__bss_start>
  400550:	75 4b                	jne    40059d <__do_global_dtors_aux+0x5d>
  400552:	bb 40 0e 60 00       	mov    $0x600e40,%ebx
  400557:	48 8b 05 e2 0a 20 00 	mov    0x200ae2(%rip),%rax        # 601040 <dtor_idx.6533>
  40055e:	48 81 eb 38 0e 60 00 	sub    $0x600e38,%rbx
  400565:	48 c1 fb 03          	sar    $0x3,%rbx
  400569:	48 83 eb 01          	sub    $0x1,%rbx
  40056d:	48 39 d8             	cmp    %rbx,%rax
  400570:	73 24                	jae    400596 <__do_global_dtors_aux+0x56>
  400572:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  400578:	48 83 c0 01          	add    $0x1,%rax
  40057c:	48 89 05 bd 0a 20 00 	mov    %rax,0x200abd(%rip)        # 601040 <dtor_idx.6533>
  400583:	ff 14 c5 38 0e 60 00 	callq  *0x600e38(,%rax,8)
  40058a:	48 8b 05 af 0a 20 00 	mov    0x200aaf(%rip),%rax        # 601040 <dtor_idx.6533>
  400591:	48 39 d8             	cmp    %rbx,%rax
  400594:	72 e2                	jb     400578 <__do_global_dtors_aux+0x38>
  400596:	c6 05 9b 0a 20 00 01 	movb   $0x1,0x200a9b(%rip)        # 601038 <__bss_start>
  40059d:	48 83 c4 08          	add    $0x8,%rsp
  4005a1:	5b                   	pop    %rbx
  4005a2:	5d                   	pop    %rbp
  4005a3:	c3                   	retq   
  4005a4:	66 66 66 2e 0f 1f 84 	data32 data32 nopw %cs:0x0(%rax,%rax,1)
  4005ab:	00 00 00 00 00 

00000000004005b0 <frame_dummy>:
  4005b0:	48 83 3d 90 08 20 00 	cmpq   $0x0,0x200890(%rip)        # 600e48 <__JCR_END__>
  4005b7:	00 
  4005b8:	55                   	push   %rbp
  4005b9:	48 89 e5             	mov    %rsp,%rbp
  4005bc:	74 12                	je     4005d0 <frame_dummy+0x20>
  4005be:	b8 00 00 00 00       	mov    $0x0,%eax
  4005c3:	48 85 c0             	test   %rax,%rax
  4005c6:	74 08                	je     4005d0 <frame_dummy+0x20>
  4005c8:	5d                   	pop    %rbp
  4005c9:	bf 48 0e 60 00       	mov    $0x600e48,%edi
  4005ce:	ff e0                	jmpq   *%rax
  4005d0:	5d                   	pop    %rbp
  4005d1:	c3                   	retq   
  4005d2:	90                   	nop
  4005d3:	90                   	nop

00000000004005d4 <DYTAN_tag>:
  4005d4:	55                   	push   %rbp
  4005d5:	48 89 e5             	mov    %rsp,%rbp
  4005d8:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4005dc:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4005e0:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
  4005e4:	5d                   	pop    %rbp
  4005e5:	c3                   	retq   

00000000004005e6 <DYTAN_display>:
  4005e6:	55                   	push   %rbp
  4005e7:	48 89 e5             	mov    %rsp,%rbp
  4005ea:	48 83 ec 58          	sub    $0x58,%rsp
  4005ee:	48 89 bd 48 ff ff ff 	mov    %rdi,-0xb8(%rbp)
  4005f5:	48 89 b5 40 ff ff ff 	mov    %rsi,-0xc0(%rbp)
  4005fc:	48 89 8d 68 ff ff ff 	mov    %rcx,-0x98(%rbp)
  400603:	4c 89 85 70 ff ff ff 	mov    %r8,-0x90(%rbp)
  40060a:	4c 89 8d 78 ff ff ff 	mov    %r9,-0x88(%rbp)
  400611:	84 c0                	test   %al,%al
  400613:	74 20                	je     400635 <DYTAN_display+0x4f>
  400615:	0f 29 45 80          	movaps %xmm0,-0x80(%rbp)
  400619:	0f 29 4d 90          	movaps %xmm1,-0x70(%rbp)
  40061d:	0f 29 55 a0          	movaps %xmm2,-0x60(%rbp)
  400621:	0f 29 5d b0          	movaps %xmm3,-0x50(%rbp)
  400625:	0f 29 65 c0          	movaps %xmm4,-0x40(%rbp)
  400629:	0f 29 6d d0          	movaps %xmm5,-0x30(%rbp)
  40062d:	0f 29 75 e0          	movaps %xmm6,-0x20(%rbp)
  400631:	0f 29 7d f0          	movaps %xmm7,-0x10(%rbp)
  400635:	48 89 95 38 ff ff ff 	mov    %rdx,-0xc8(%rbp)
  40063c:	c9                   	leaveq 
  40063d:	c3                   	retq   

000000000040063e <main>:
  40063e:	55                   	push   %rbp
  40063f:	48 89 e5             	mov    %rsp,%rbp
  400642:	48 83 ec 30          	sub    $0x30,%rsp
  400646:	89 7d dc             	mov    %edi,-0x24(%rbp)
  400649:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
  40064d:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
  400654:	c7 45 f8 01 00 00 00 	movl   $0x1,-0x8(%rbp)
  40065b:	e9 ce 00 00 00       	jmpq   40072e <main+0xf0>
  400660:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%rbp)
  400667:	ba cc 08 40 00       	mov    $0x4008cc,%edx
  40066c:	8b 45 f8             	mov    -0x8(%rbp),%eax
  40066f:	48 98                	cltq   
  400671:	48 c1 e0 03          	shl    $0x3,%rax
  400675:	48 03 45 d0          	add    -0x30(%rbp),%rax
  400679:	48 8b 00             	mov    (%rax),%rax
  40067c:	48 89 d6             	mov    %rdx,%rsi
  40067f:	48 89 c7             	mov    %rax,%rdi
  400682:	e8 59 fe ff ff       	callq  4004e0 <fopen@plt>
  400687:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  40068b:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  40068f:	48 89 c7             	mov    %rax,%rdi
  400692:	e8 29 fe ff ff       	callq  4004c0 <fgetc@plt>
  400697:	88 45 fe             	mov    %al,-0x2(%rbp)
  40069a:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
  40069e:	88 45 ff             	mov    %al,-0x1(%rbp)
  4006a1:	eb 20                	jmp    4006c3 <main+0x85>
  4006a3:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
  4006a7:	3c 0a                	cmp    $0xa,%al
  4006a9:	75 09                	jne    4006b4 <main+0x76>
  4006ab:	8b 45 f0             	mov    -0x10(%rbp),%eax
  4006ae:	83 c0 01             	add    $0x1,%eax
  4006b1:	89 45 f0             	mov    %eax,-0x10(%rbp)
  4006b4:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4006b8:	48 89 c7             	mov    %rax,%rdi
  4006bb:	e8 00 fe ff ff       	callq  4004c0 <fgetc@plt>
  4006c0:	88 45 fe             	mov    %al,-0x2(%rbp)
  4006c3:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
  4006c7:	3c ff                	cmp    $0xff,%al
  4006c9:	75 d8                	jne    4006a3 <main+0x65>
  4006cb:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4006cf:	48 89 c7             	mov    %rax,%rdi
  4006d2:	e8 c9 fd ff ff       	callq  4004a0 <fclose@plt>
  4006d7:	8b 55 f0             	mov    -0x10(%rbp),%edx
  4006da:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
  4006de:	89 d1                	mov    %edx,%ecx
  4006e0:	ba ce 08 40 00       	mov    $0x4008ce,%edx
  4006e5:	be 04 00 00 00       	mov    $0x4,%esi
  4006ea:	48 89 c7             	mov    %rax,%rdi
  4006ed:	b8 00 00 00 00       	mov    $0x0,%eax
  4006f2:	e8 ef fe ff ff       	callq  4005e6 <DYTAN_display>
  4006f7:	8b 55 f0             	mov    -0x10(%rbp),%edx
  4006fa:	8b 45 f8             	mov    -0x8(%rbp),%eax
  4006fd:	48 98                	cltq   
  4006ff:	48 c1 e0 03          	shl    $0x3,%rax
  400703:	48 03 45 d0          	add    -0x30(%rbp),%rax
  400707:	48 8b 08             	mov    (%rax),%rcx
  40070a:	b8 d7 08 40 00       	mov    $0x4008d7,%eax
  40070f:	48 89 ce             	mov    %rcx,%rsi
  400712:	48 89 c7             	mov    %rax,%rdi
  400715:	b8 00 00 00 00       	mov    $0x0,%eax
  40071a:	e8 91 fd ff ff       	callq  4004b0 <printf@plt>
  40071f:	8b 55 f4             	mov    -0xc(%rbp),%edx
  400722:	8b 45 f0             	mov    -0x10(%rbp),%eax
  400725:	01 d0                	add    %edx,%eax
  400727:	89 45 f4             	mov    %eax,-0xc(%rbp)
  40072a:	83 45 f8 01          	addl   $0x1,-0x8(%rbp)
  40072e:	8b 45 f8             	mov    -0x8(%rbp),%eax
  400731:	3b 45 dc             	cmp    -0x24(%rbp),%eax
  400734:	0f 8c 26 ff ff ff    	jl     400660 <main+0x22>
  40073a:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
  40073e:	0f be d0             	movsbl %al,%edx
  400741:	48 8d 45 fe          	lea    -0x2(%rbp),%rax
  400745:	89 d1                	mov    %edx,%ecx
  400747:	ba df 08 40 00       	mov    $0x4008df,%edx
  40074c:	be 01 00 00 00       	mov    $0x1,%esi
  400751:	48 89 c7             	mov    %rax,%rdi
  400754:	b8 00 00 00 00       	mov    $0x0,%eax
  400759:	e8 88 fe ff ff       	callq  4005e6 <DYTAN_display>
  40075e:	0f b6 45 ff          	movzbl -0x1(%rbp),%eax
  400762:	0f be d0             	movsbl %al,%edx
  400765:	48 8d 45 ff          	lea    -0x1(%rbp),%rax
  400769:	89 d1                	mov    %edx,%ecx
  40076b:	ba e4 08 40 00       	mov    $0x4008e4,%edx
  400770:	be 01 00 00 00       	mov    $0x1,%esi
  400775:	48 89 c7             	mov    %rax,%rdi
  400778:	b8 00 00 00 00       	mov    $0x0,%eax
  40077d:	e8 64 fe ff ff       	callq  4005e6 <DYTAN_display>
  400782:	8b 55 f0             	mov    -0x10(%rbp),%edx
  400785:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
  400789:	89 d1                	mov    %edx,%ecx
  40078b:	ba ce 08 40 00       	mov    $0x4008ce,%edx
  400790:	be 04 00 00 00       	mov    $0x4,%esi
  400795:	48 89 c7             	mov    %rax,%rdi
  400798:	b8 00 00 00 00       	mov    $0x0,%eax
  40079d:	e8 44 fe ff ff       	callq  4005e6 <DYTAN_display>
  4007a2:	8b 55 f4             	mov    -0xc(%rbp),%edx
  4007a5:	48 8d 45 f4          	lea    -0xc(%rbp),%rax
  4007a9:	89 d1                	mov    %edx,%ecx
  4007ab:	ba e9 08 40 00       	mov    $0x4008e9,%edx
  4007b0:	be 04 00 00 00       	mov    $0x4,%esi
  4007b5:	48 89 c7             	mov    %rax,%rdi
  4007b8:	b8 00 00 00 00       	mov    $0x0,%eax
  4007bd:	e8 24 fe ff ff       	callq  4005e6 <DYTAN_display>
  4007c2:	8b 55 f4             	mov    -0xc(%rbp),%edx
  4007c5:	b8 f8 08 40 00       	mov    $0x4008f8,%eax
  4007ca:	89 d6                	mov    %edx,%esi
  4007cc:	48 89 c7             	mov    %rax,%rdi
  4007cf:	b8 00 00 00 00       	mov    $0x0,%eax
  4007d4:	e8 d7 fc ff ff       	callq  4004b0 <printf@plt>
  4007d9:	b8 00 00 00 00       	mov    $0x0,%eax
  4007de:	c9                   	leaveq 
  4007df:	c3                   	retq   

00000000004007e0 <__libc_csu_init>:
  4007e0:	48 89 6c 24 d8       	mov    %rbp,-0x28(%rsp)
  4007e5:	4c 89 64 24 e0       	mov    %r12,-0x20(%rsp)
  4007ea:	48 8d 2d 33 06 20 00 	lea    0x200633(%rip),%rbp        # 600e24 <__init_array_end>
  4007f1:	4c 8d 25 2c 06 20 00 	lea    0x20062c(%rip),%r12        # 600e24 <__init_array_end>
  4007f8:	4c 89 6c 24 e8       	mov    %r13,-0x18(%rsp)
  4007fd:	4c 89 74 24 f0       	mov    %r14,-0x10(%rsp)
  400802:	4c 89 7c 24 f8       	mov    %r15,-0x8(%rsp)
  400807:	48 89 5c 24 d0       	mov    %rbx,-0x30(%rsp)
  40080c:	48 83 ec 38          	sub    $0x38,%rsp
  400810:	4c 29 e5             	sub    %r12,%rbp
  400813:	41 89 fd             	mov    %edi,%r13d
  400816:	49 89 f6             	mov    %rsi,%r14
  400819:	48 c1 fd 03          	sar    $0x3,%rbp
  40081d:	49 89 d7             	mov    %rdx,%r15
  400820:	e8 4b fc ff ff       	callq  400470 <_init>
  400825:	48 85 ed             	test   %rbp,%rbp
  400828:	74 1c                	je     400846 <__libc_csu_init+0x66>
  40082a:	31 db                	xor    %ebx,%ebx
  40082c:	0f 1f 40 00          	nopl   0x0(%rax)
  400830:	4c 89 fa             	mov    %r15,%rdx
  400833:	4c 89 f6             	mov    %r14,%rsi
  400836:	44 89 ef             	mov    %r13d,%edi
  400839:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40083d:	48 83 c3 01          	add    $0x1,%rbx
  400841:	48 39 eb             	cmp    %rbp,%rbx
  400844:	75 ea                	jne    400830 <__libc_csu_init+0x50>
  400846:	48 8b 5c 24 08       	mov    0x8(%rsp),%rbx
  40084b:	48 8b 6c 24 10       	mov    0x10(%rsp),%rbp
  400850:	4c 8b 64 24 18       	mov    0x18(%rsp),%r12
  400855:	4c 8b 6c 24 20       	mov    0x20(%rsp),%r13
  40085a:	4c 8b 74 24 28       	mov    0x28(%rsp),%r14
  40085f:	4c 8b 7c 24 30       	mov    0x30(%rsp),%r15
  400864:	48 83 c4 38          	add    $0x38,%rsp
  400868:	c3                   	retq   
  400869:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400870 <__libc_csu_fini>:
  400870:	f3 c3                	repz retq 
  400872:	90                   	nop
  400873:	90                   	nop
  400874:	90                   	nop
  400875:	90                   	nop
  400876:	90                   	nop
  400877:	90                   	nop
  400878:	90                   	nop
  400879:	90                   	nop
  40087a:	90                   	nop
  40087b:	90                   	nop
  40087c:	90                   	nop
  40087d:	90                   	nop
  40087e:	90                   	nop
  40087f:	90                   	nop

0000000000400880 <__do_global_ctors_aux>:
  400880:	55                   	push   %rbp
  400881:	48 89 e5             	mov    %rsp,%rbp
  400884:	53                   	push   %rbx
  400885:	48 83 ec 08          	sub    $0x8,%rsp
  400889:	48 8b 05 98 05 20 00 	mov    0x200598(%rip),%rax        # 600e28 <__CTOR_LIST__>
  400890:	48 83 f8 ff          	cmp    $0xffffffffffffffff,%rax
  400894:	74 19                	je     4008af <__do_global_ctors_aux+0x2f>
  400896:	bb 28 0e 60 00       	mov    $0x600e28,%ebx
  40089b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4008a0:	48 83 eb 08          	sub    $0x8,%rbx
  4008a4:	ff d0                	callq  *%rax
  4008a6:	48 8b 03             	mov    (%rbx),%rax
  4008a9:	48 83 f8 ff          	cmp    $0xffffffffffffffff,%rax
  4008ad:	75 f1                	jne    4008a0 <__do_global_ctors_aux+0x20>
  4008af:	48 83 c4 08          	add    $0x8,%rsp
  4008b3:	5b                   	pop    %rbx
  4008b4:	5d                   	pop    %rbp
  4008b5:	c3                   	retq   
  4008b6:	90                   	nop
  4008b7:	90                   	nop

Disassembly of section .fini:

00000000004008b8 <_fini>:
  4008b8:	48 83 ec 08          	sub    $0x8,%rsp
  4008bc:	e8 7f fc ff ff       	callq  400540 <__do_global_dtors_aux>
  4008c1:	48 83 c4 08          	add    $0x8,%rsp
  4008c5:	c3                   	retq   

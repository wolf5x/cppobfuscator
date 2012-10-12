
./a.out:     file format elf32-i386


Disassembly of section .init:

0804832c <_init>:
 804832c:	55                   	push   %ebp
 804832d:	89 e5                	mov    %esp,%ebp
 804832f:	53                   	push   %ebx
 8048330:	83 ec 04             	sub    $0x4,%esp
 8048333:	e8 00 00 00 00       	call   8048338 <_init+0xc>
 8048338:	5b                   	pop    %ebx
 8048339:	81 c3 bc 1c 00 00    	add    $0x1cbc,%ebx
 804833f:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 8048345:	85 d2                	test   %edx,%edx
 8048347:	74 05                	je     804834e <_init+0x22>
 8048349:	e8 22 00 00 00       	call   8048370 <__gmon_start__@plt>
 804834e:	e8 dd 00 00 00       	call   8048430 <frame_dummy>
 8048353:	e8 c8 01 00 00       	call   8048520 <__do_global_ctors_aux>
 8048358:	58                   	pop    %eax
 8048359:	5b                   	pop    %ebx
 804835a:	c9                   	leave  
 804835b:	c3                   	ret    

Disassembly of section .plt:

08048360 <__gmon_start__@plt-0x10>:
 8048360:	ff 35 f8 9f 04 08    	pushl  0x8049ff8
 8048366:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 804836c:	00 00                	add    %al,(%eax)
	...

08048370 <__gmon_start__@plt>:
 8048370:	ff 25 00 a0 04 08    	jmp    *0x804a000
 8048376:	68 00 00 00 00       	push   $0x0
 804837b:	e9 e0 ff ff ff       	jmp    8048360 <_init+0x34>

08048380 <__libc_start_main@plt>:
 8048380:	ff 25 04 a0 04 08    	jmp    *0x804a004
 8048386:	68 08 00 00 00       	push   $0x8
 804838b:	e9 d0 ff ff ff       	jmp    8048360 <_init+0x34>

08048390 <printf@plt>:
 8048390:	ff 25 08 a0 04 08    	jmp    *0x804a008
 8048396:	68 10 00 00 00       	push   $0x10
 804839b:	e9 c0 ff ff ff       	jmp    8048360 <_init+0x34>

Disassembly of section .text:

080483a0 <_start>:
 80483a0:	31 ed                	xor    %ebp,%ebp
 80483a2:	5e                   	pop    %esi
 80483a3:	89 e1                	mov    %esp,%ecx
 80483a5:	83 e4 f0             	and    $0xfffffff0,%esp
 80483a8:	50                   	push   %eax
 80483a9:	54                   	push   %esp
 80483aa:	52                   	push   %edx
 80483ab:	68 b0 84 04 08       	push   $0x80484b0
 80483b0:	68 c0 84 04 08       	push   $0x80484c0
 80483b5:	51                   	push   %ecx
 80483b6:	56                   	push   %esi
 80483b7:	68 59 84 04 08       	push   $0x8048459
 80483bc:	e8 bf ff ff ff       	call   8048380 <__libc_start_main@plt>
 80483c1:	f4                   	hlt    
 80483c2:	90                   	nop
 80483c3:	90                   	nop
 80483c4:	90                   	nop
 80483c5:	90                   	nop
 80483c6:	90                   	nop
 80483c7:	90                   	nop
 80483c8:	90                   	nop
 80483c9:	90                   	nop
 80483ca:	90                   	nop
 80483cb:	90                   	nop
 80483cc:	90                   	nop
 80483cd:	90                   	nop
 80483ce:	90                   	nop
 80483cf:	90                   	nop

080483d0 <__do_global_dtors_aux>:
 80483d0:	55                   	push   %ebp
 80483d1:	89 e5                	mov    %esp,%ebp
 80483d3:	53                   	push   %ebx
 80483d4:	83 ec 04             	sub    $0x4,%esp
 80483d7:	80 3d 14 a0 04 08 00 	cmpb   $0x0,0x804a014
 80483de:	75 3f                	jne    804841f <__do_global_dtors_aux+0x4f>
 80483e0:	a1 18 a0 04 08       	mov    0x804a018,%eax
 80483e5:	bb 00 9f 04 08       	mov    $0x8049f00,%ebx
 80483ea:	81 eb fc 9e 04 08    	sub    $0x8049efc,%ebx
 80483f0:	c1 fb 02             	sar    $0x2,%ebx
 80483f3:	83 eb 01             	sub    $0x1,%ebx
 80483f6:	39 d8                	cmp    %ebx,%eax
 80483f8:	73 1e                	jae    8048418 <__do_global_dtors_aux+0x48>
 80483fa:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048400:	83 c0 01             	add    $0x1,%eax
 8048403:	a3 18 a0 04 08       	mov    %eax,0x804a018
 8048408:	ff 14 85 fc 9e 04 08 	call   *0x8049efc(,%eax,4)
 804840f:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048414:	39 d8                	cmp    %ebx,%eax
 8048416:	72 e8                	jb     8048400 <__do_global_dtors_aux+0x30>
 8048418:	c6 05 14 a0 04 08 01 	movb   $0x1,0x804a014
 804841f:	83 c4 04             	add    $0x4,%esp
 8048422:	5b                   	pop    %ebx
 8048423:	5d                   	pop    %ebp
 8048424:	c3                   	ret    
 8048425:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8048429:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048430 <frame_dummy>:
 8048430:	55                   	push   %ebp
 8048431:	89 e5                	mov    %esp,%ebp
 8048433:	83 ec 18             	sub    $0x18,%esp
 8048436:	a1 04 9f 04 08       	mov    0x8049f04,%eax
 804843b:	85 c0                	test   %eax,%eax
 804843d:	74 12                	je     8048451 <frame_dummy+0x21>
 804843f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048444:	85 c0                	test   %eax,%eax
 8048446:	74 09                	je     8048451 <frame_dummy+0x21>
 8048448:	c7 04 24 04 9f 04 08 	movl   $0x8049f04,(%esp)
 804844f:	ff d0                	call   *%eax
 8048451:	c9                   	leave  
 8048452:	c3                   	ret    
 8048453:	90                   	nop

08048454 <_Z4funci>:
 8048454:	55                   	push   %ebp
 8048455:	89 e5                	mov    %esp,%ebp
 8048457:	5d                   	pop    %ebp
 8048458:	c3                   	ret    

08048459 <main>:
 8048459:	55                   	push   %ebp
 804845a:	89 e5                	mov    %esp,%ebp
 804845c:	83 e4 f0             	and    $0xfffffff0,%esp
 804845f:	83 ec 30             	sub    $0x30,%esp
 8048462:	c7 44 24 1c 54 84 04 	movl   $0x8048454,0x1c(%esp)
 8048469:	08 
 804846a:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 804846e:	89 44 24 24          	mov    %eax,0x24(%esp)
 8048472:	c7 44 24 20 02 00 00 	movl   $0x2,0x20(%esp)
 8048479:	00 
 804847a:	c7 44 24 28 03 00 00 	movl   $0x3,0x28(%esp)
 8048481:	00 
 8048482:	8d 44 24 20          	lea    0x20(%esp),%eax
 8048486:	89 44 24 2c          	mov    %eax,0x2c(%esp)
 804848a:	8b 44 24 2c          	mov    0x2c(%esp),%eax
 804848e:	83 e8 04             	sub    $0x4,%eax
 8048491:	8b 00                	mov    (%eax),%eax
 8048493:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048497:	c7 04 24 70 85 04 08 	movl   $0x8048570,(%esp)
 804849e:	e8 ed fe ff ff       	call   8048390 <printf@plt>
 80484a3:	b8 00 00 00 00       	mov    $0x0,%eax
 80484a8:	c9                   	leave  
 80484a9:	c3                   	ret    
 80484aa:	90                   	nop
 80484ab:	90                   	nop
 80484ac:	90                   	nop
 80484ad:	90                   	nop
 80484ae:	90                   	nop
 80484af:	90                   	nop

080484b0 <__libc_csu_fini>:
 80484b0:	55                   	push   %ebp
 80484b1:	89 e5                	mov    %esp,%ebp
 80484b3:	5d                   	pop    %ebp
 80484b4:	c3                   	ret    
 80484b5:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80484b9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

080484c0 <__libc_csu_init>:
 80484c0:	55                   	push   %ebp
 80484c1:	89 e5                	mov    %esp,%ebp
 80484c3:	57                   	push   %edi
 80484c4:	56                   	push   %esi
 80484c5:	53                   	push   %ebx
 80484c6:	e8 4f 00 00 00       	call   804851a <__i686.get_pc_thunk.bx>
 80484cb:	81 c3 29 1b 00 00    	add    $0x1b29,%ebx
 80484d1:	83 ec 1c             	sub    $0x1c,%esp
 80484d4:	e8 53 fe ff ff       	call   804832c <_init>
 80484d9:	8d bb 00 ff ff ff    	lea    -0x100(%ebx),%edi
 80484df:	8d 83 00 ff ff ff    	lea    -0x100(%ebx),%eax
 80484e5:	29 c7                	sub    %eax,%edi
 80484e7:	c1 ff 02             	sar    $0x2,%edi
 80484ea:	85 ff                	test   %edi,%edi
 80484ec:	74 24                	je     8048512 <__libc_csu_init+0x52>
 80484ee:	31 f6                	xor    %esi,%esi
 80484f0:	8b 45 10             	mov    0x10(%ebp),%eax
 80484f3:	89 44 24 08          	mov    %eax,0x8(%esp)
 80484f7:	8b 45 0c             	mov    0xc(%ebp),%eax
 80484fa:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484fe:	8b 45 08             	mov    0x8(%ebp),%eax
 8048501:	89 04 24             	mov    %eax,(%esp)
 8048504:	ff 94 b3 00 ff ff ff 	call   *-0x100(%ebx,%esi,4)
 804850b:	83 c6 01             	add    $0x1,%esi
 804850e:	39 fe                	cmp    %edi,%esi
 8048510:	72 de                	jb     80484f0 <__libc_csu_init+0x30>
 8048512:	83 c4 1c             	add    $0x1c,%esp
 8048515:	5b                   	pop    %ebx
 8048516:	5e                   	pop    %esi
 8048517:	5f                   	pop    %edi
 8048518:	5d                   	pop    %ebp
 8048519:	c3                   	ret    

0804851a <__i686.get_pc_thunk.bx>:
 804851a:	8b 1c 24             	mov    (%esp),%ebx
 804851d:	c3                   	ret    
 804851e:	90                   	nop
 804851f:	90                   	nop

08048520 <__do_global_ctors_aux>:
 8048520:	55                   	push   %ebp
 8048521:	89 e5                	mov    %esp,%ebp
 8048523:	53                   	push   %ebx
 8048524:	83 ec 04             	sub    $0x4,%esp
 8048527:	a1 f4 9e 04 08       	mov    0x8049ef4,%eax
 804852c:	83 f8 ff             	cmp    $0xffffffff,%eax
 804852f:	74 13                	je     8048544 <__do_global_ctors_aux+0x24>
 8048531:	bb f4 9e 04 08       	mov    $0x8049ef4,%ebx
 8048536:	66 90                	xchg   %ax,%ax
 8048538:	83 eb 04             	sub    $0x4,%ebx
 804853b:	ff d0                	call   *%eax
 804853d:	8b 03                	mov    (%ebx),%eax
 804853f:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048542:	75 f4                	jne    8048538 <__do_global_ctors_aux+0x18>
 8048544:	83 c4 04             	add    $0x4,%esp
 8048547:	5b                   	pop    %ebx
 8048548:	5d                   	pop    %ebp
 8048549:	c3                   	ret    
 804854a:	90                   	nop
 804854b:	90                   	nop

Disassembly of section .fini:

0804854c <_fini>:
 804854c:	55                   	push   %ebp
 804854d:	89 e5                	mov    %esp,%ebp
 804854f:	53                   	push   %ebx
 8048550:	83 ec 04             	sub    $0x4,%esp
 8048553:	e8 00 00 00 00       	call   8048558 <_fini+0xc>
 8048558:	5b                   	pop    %ebx
 8048559:	81 c3 9c 1a 00 00    	add    $0x1a9c,%ebx
 804855f:	e8 6c fe ff ff       	call   80483d0 <__do_global_dtors_aux>
 8048564:	59                   	pop    %ecx
 8048565:	5b                   	pop    %ebx
 8048566:	c9                   	leave  
 8048567:	c3                   	ret    

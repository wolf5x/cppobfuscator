
./a.out:     file format elf32-i386


Disassembly of section .init:

08048308 <_init>:
 8048308:	55                   	push   %ebp
 8048309:	89 e5                	mov    %esp,%ebp
 804830b:	53                   	push   %ebx
 804830c:	83 ec 04             	sub    $0x4,%esp
 804830f:	e8 00 00 00 00       	call   8048314 <_init+0xc>
 8048314:	5b                   	pop    %ebx
 8048315:	81 c3 e0 1c 00 00    	add    $0x1ce0,%ebx
 804831b:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 8048321:	85 d2                	test   %edx,%edx
 8048323:	74 05                	je     804832a <_init+0x22>
 8048325:	e8 26 00 00 00       	call   8048350 <__gmon_start__@plt>
 804832a:	e8 d1 00 00 00       	call   8048400 <frame_dummy>
 804832f:	e8 ac 01 00 00       	call   80484e0 <__do_global_ctors_aux>
 8048334:	58                   	pop    %eax
 8048335:	5b                   	pop    %ebx
 8048336:	c9                   	leave  
 8048337:	c3                   	ret    

Disassembly of section .plt:

08048340 <__gmon_start__@plt-0x10>:
 8048340:	ff 35 f8 9f 04 08    	pushl  0x8049ff8
 8048346:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 804834c:	00 00                	add    %al,(%eax)
	...

08048350 <__gmon_start__@plt>:
 8048350:	ff 25 00 a0 04 08    	jmp    *0x804a000
 8048356:	68 00 00 00 00       	push   $0x0
 804835b:	e9 e0 ff ff ff       	jmp    8048340 <_init+0x38>

08048360 <__libc_start_main@plt>:
 8048360:	ff 25 04 a0 04 08    	jmp    *0x804a004
 8048366:	68 08 00 00 00       	push   $0x8
 804836b:	e9 d0 ff ff ff       	jmp    8048340 <_init+0x38>

Disassembly of section .text:

08048370 <_start>:
 8048370:	31 ed                	xor    %ebp,%ebp
 8048372:	5e                   	pop    %esi
 8048373:	89 e1                	mov    %esp,%ecx
 8048375:	83 e4 f0             	and    $0xfffffff0,%esp
 8048378:	50                   	push   %eax
 8048379:	54                   	push   %esp
 804837a:	52                   	push   %edx
 804837b:	68 70 84 04 08       	push   $0x8048470
 8048380:	68 80 84 04 08       	push   $0x8048480
 8048385:	51                   	push   %ecx
 8048386:	56                   	push   %esi
 8048387:	68 24 84 04 08       	push   $0x8048424
 804838c:	e8 cf ff ff ff       	call   8048360 <__libc_start_main@plt>
 8048391:	f4                   	hlt    
 8048392:	90                   	nop
 8048393:	90                   	nop
 8048394:	90                   	nop
 8048395:	90                   	nop
 8048396:	90                   	nop
 8048397:	90                   	nop
 8048398:	90                   	nop
 8048399:	90                   	nop
 804839a:	90                   	nop
 804839b:	90                   	nop
 804839c:	90                   	nop
 804839d:	90                   	nop
 804839e:	90                   	nop
 804839f:	90                   	nop

080483a0 <__do_global_dtors_aux>:
 80483a0:	55                   	push   %ebp
 80483a1:	89 e5                	mov    %esp,%ebp
 80483a3:	53                   	push   %ebx
 80483a4:	83 ec 04             	sub    $0x4,%esp
 80483a7:	80 3d 10 a0 04 08 00 	cmpb   $0x0,0x804a010
 80483ae:	75 3f                	jne    80483ef <__do_global_dtors_aux+0x4f>
 80483b0:	a1 14 a0 04 08       	mov    0x804a014,%eax
 80483b5:	bb 00 9f 04 08       	mov    $0x8049f00,%ebx
 80483ba:	81 eb fc 9e 04 08    	sub    $0x8049efc,%ebx
 80483c0:	c1 fb 02             	sar    $0x2,%ebx
 80483c3:	83 eb 01             	sub    $0x1,%ebx
 80483c6:	39 d8                	cmp    %ebx,%eax
 80483c8:	73 1e                	jae    80483e8 <__do_global_dtors_aux+0x48>
 80483ca:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 80483d0:	83 c0 01             	add    $0x1,%eax
 80483d3:	a3 14 a0 04 08       	mov    %eax,0x804a014
 80483d8:	ff 14 85 fc 9e 04 08 	call   *0x8049efc(,%eax,4)
 80483df:	a1 14 a0 04 08       	mov    0x804a014,%eax
 80483e4:	39 d8                	cmp    %ebx,%eax
 80483e6:	72 e8                	jb     80483d0 <__do_global_dtors_aux+0x30>
 80483e8:	c6 05 10 a0 04 08 01 	movb   $0x1,0x804a010
 80483ef:	83 c4 04             	add    $0x4,%esp
 80483f2:	5b                   	pop    %ebx
 80483f3:	5d                   	pop    %ebp
 80483f4:	c3                   	ret    
 80483f5:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80483f9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048400 <frame_dummy>:
 8048400:	55                   	push   %ebp
 8048401:	89 e5                	mov    %esp,%ebp
 8048403:	83 ec 18             	sub    $0x18,%esp
 8048406:	a1 04 9f 04 08       	mov    0x8049f04,%eax
 804840b:	85 c0                	test   %eax,%eax
 804840d:	74 12                	je     8048421 <frame_dummy+0x21>
 804840f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048414:	85 c0                	test   %eax,%eax
 8048416:	74 09                	je     8048421 <frame_dummy+0x21>
 8048418:	c7 04 24 04 9f 04 08 	movl   $0x8049f04,(%esp)
 804841f:	ff d0                	call   *%eax
 8048421:	c9                   	leave  
 8048422:	c3                   	ret    
 8048423:	90                   	nop

08048424 <main>:
 8048424:	55                   	push   %ebp
 8048425:	89 e5                	mov    %esp,%ebp
 8048427:	83 ec 10             	sub    $0x10,%esp
 804842a:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%ebp)
 8048431:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048434:	83 f8 03             	cmp    $0x3,%eax
 8048437:	7f 0b                	jg     8048444 <main+0x20>
 8048439:	83 f8 01             	cmp    $0x1,%eax
 804843c:	7d 2b                	jge    8048469 <main+0x45>
 804843e:	85 c0                	test   %eax,%eax
 8048440:	74 09                	je     804844b <main+0x27>
 8048442:	eb 1d                	jmp    8048461 <main+0x3d>
 8048444:	83 f8 04             	cmp    $0x4,%eax
 8048447:	74 0b                	je     8048454 <main+0x30>
 8048449:	eb 16                	jmp    8048461 <main+0x3d>
 804844b:	c7 45 fc 02 00 00 00 	movl   $0x2,-0x4(%ebp)
 8048452:	eb 15                	jmp    8048469 <main+0x45>
 8048454:	83 6d f8 02          	subl   $0x2,-0x8(%ebp)
 8048458:	c7 45 fc 03 00 00 00 	movl   $0x3,-0x4(%ebp)
 804845f:	eb 09                	jmp    804846a <main+0x46>
 8048461:	90                   	nop
 8048462:	b8 00 00 00 00       	mov    $0x0,%eax
 8048467:	c9                   	leave  
 8048468:	c3                   	ret    
 8048469:	90                   	nop
 804846a:	eb c5                	jmp    8048431 <main+0xd>
 804846c:	90                   	nop
 804846d:	90                   	nop
 804846e:	90                   	nop
 804846f:	90                   	nop

08048470 <__libc_csu_fini>:
 8048470:	55                   	push   %ebp
 8048471:	89 e5                	mov    %esp,%ebp
 8048473:	5d                   	pop    %ebp
 8048474:	c3                   	ret    
 8048475:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8048479:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048480 <__libc_csu_init>:
 8048480:	55                   	push   %ebp
 8048481:	89 e5                	mov    %esp,%ebp
 8048483:	57                   	push   %edi
 8048484:	56                   	push   %esi
 8048485:	53                   	push   %ebx
 8048486:	e8 4f 00 00 00       	call   80484da <__i686.get_pc_thunk.bx>
 804848b:	81 c3 69 1b 00 00    	add    $0x1b69,%ebx
 8048491:	83 ec 1c             	sub    $0x1c,%esp
 8048494:	e8 6f fe ff ff       	call   8048308 <_init>
 8048499:	8d bb 00 ff ff ff    	lea    -0x100(%ebx),%edi
 804849f:	8d 83 00 ff ff ff    	lea    -0x100(%ebx),%eax
 80484a5:	29 c7                	sub    %eax,%edi
 80484a7:	c1 ff 02             	sar    $0x2,%edi
 80484aa:	85 ff                	test   %edi,%edi
 80484ac:	74 24                	je     80484d2 <__libc_csu_init+0x52>
 80484ae:	31 f6                	xor    %esi,%esi
 80484b0:	8b 45 10             	mov    0x10(%ebp),%eax
 80484b3:	89 44 24 08          	mov    %eax,0x8(%esp)
 80484b7:	8b 45 0c             	mov    0xc(%ebp),%eax
 80484ba:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484be:	8b 45 08             	mov    0x8(%ebp),%eax
 80484c1:	89 04 24             	mov    %eax,(%esp)
 80484c4:	ff 94 b3 00 ff ff ff 	call   *-0x100(%ebx,%esi,4)
 80484cb:	83 c6 01             	add    $0x1,%esi
 80484ce:	39 fe                	cmp    %edi,%esi
 80484d0:	72 de                	jb     80484b0 <__libc_csu_init+0x30>
 80484d2:	83 c4 1c             	add    $0x1c,%esp
 80484d5:	5b                   	pop    %ebx
 80484d6:	5e                   	pop    %esi
 80484d7:	5f                   	pop    %edi
 80484d8:	5d                   	pop    %ebp
 80484d9:	c3                   	ret    

080484da <__i686.get_pc_thunk.bx>:
 80484da:	8b 1c 24             	mov    (%esp),%ebx
 80484dd:	c3                   	ret    
 80484de:	90                   	nop
 80484df:	90                   	nop

080484e0 <__do_global_ctors_aux>:
 80484e0:	55                   	push   %ebp
 80484e1:	89 e5                	mov    %esp,%ebp
 80484e3:	53                   	push   %ebx
 80484e4:	83 ec 04             	sub    $0x4,%esp
 80484e7:	a1 f4 9e 04 08       	mov    0x8049ef4,%eax
 80484ec:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484ef:	74 13                	je     8048504 <__do_global_ctors_aux+0x24>
 80484f1:	bb f4 9e 04 08       	mov    $0x8049ef4,%ebx
 80484f6:	66 90                	xchg   %ax,%ax
 80484f8:	83 eb 04             	sub    $0x4,%ebx
 80484fb:	ff d0                	call   *%eax
 80484fd:	8b 03                	mov    (%ebx),%eax
 80484ff:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048502:	75 f4                	jne    80484f8 <__do_global_ctors_aux+0x18>
 8048504:	83 c4 04             	add    $0x4,%esp
 8048507:	5b                   	pop    %ebx
 8048508:	5d                   	pop    %ebp
 8048509:	c3                   	ret    
 804850a:	90                   	nop
 804850b:	90                   	nop

Disassembly of section .fini:

0804850c <_fini>:
 804850c:	55                   	push   %ebp
 804850d:	89 e5                	mov    %esp,%ebp
 804850f:	53                   	push   %ebx
 8048510:	83 ec 04             	sub    $0x4,%esp
 8048513:	e8 00 00 00 00       	call   8048518 <_fini+0xc>
 8048518:	5b                   	pop    %ebx
 8048519:	81 c3 dc 1a 00 00    	add    $0x1adc,%ebx
 804851f:	e8 7c fe ff ff       	call   80483a0 <__do_global_dtors_aux>
 8048524:	59                   	pop    %ecx
 8048525:	5b                   	pop    %ebx
 8048526:	c9                   	leave  
 8048527:	c3                   	ret    

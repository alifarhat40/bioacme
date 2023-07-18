      program mxexp
*
*---  sample program illustrating the use of DGEXPV ...
*     Non-symmetric problem (Example 6.3 in the Expokit report) ...
*
      implicit none
      external dgcoov, dgcrsv, dgccsv

      double precision tic, tac, clock

*---  matrix data ... 
*---  BEWARE: these values should match those in dgmatv.f
      integer n, nz, nmax, nzmax
      !parameter( nmax = 5000000, nzmax = 60000000 )
      
*---  nzmax must match all other instances of the RMAT common block 
      parameter( nmax =   500000, nzmax = 60000000 )
      integer ia(nzmax), ja(nzmax)
      double precision a(nzmax)
      common /RMAT/ a, ia, ja, nz, n

*---  arguments variables ...
      integer m, mmax, lwsp, liwsp
      parameter( mmax = 50 )
      parameter( lwsp = nmax*(mmax+2)+5*(mmax+2)**2+7, liwsp = nmax )
      integer iwsp(liwsp)
      double precision t, tol, anorm
      double precision v(nmax), w(nmax), wsp(lwsp)

      integer i, j, itrace, iflag
      double precision ZERO, ONE
      parameter( ZERO=0.0d0, ONE=1.0d0 )

      intrinsic ABS
*
*---  Executable statements ...
c---  handle command line parameters
      integer numarg, nsteps
      character(len=256) :: arg
      character tm*256
      character times*256
      character dts*256
      integer   p0
      character p0file*256
      character dynfn*256
      character dyndir*256
      double precision time
      double precision dt
      double precision maxdiag

      i = command_argument_count()
      if (i == 0) then
        call printUsage()
      endif

      tm = ''
      p0 = 0
      dyndir = './'
      time = 100
      dt = 10
c     Parsing command line
      do i = 1, command_argument_count()
        call get_command_argument(i, arg)
        select case(arg)
          case ('-m', '--tm')
            call get_command_argument(i+1, tm)
            
          case ('-t', '--time')
            call get_command_argument(i+1, times)
            read(times,*) time

          case ('-d', '--dt')
            call get_command_argument(i+1, dts)
            read(dts,*) dt

          case ('-p', '--p0')
            call get_command_argument(i+1, p0file)

          case ('-i', '--intermediate_dir')
            call get_command_argument(i+1, dyndir)

        end select
      end do

      if (LEN_TRIM(tm) == 0) then
        stop 'Please input rate matrix file.'
      endif

      if (LEN_TRIM(p0file) == 1) then
        read(p0file,*) p0
      else
        p0 = 2
      endif


*---  load the matrix ...
      n = nmax
      nz = nzmax
      open(UNIT=7,STATUS='old',IOSTAT=iflag,FILE=tm)
      if ( iflag.ne.0 ) stop 'Error - matrix could not be loaded (1).'
      read( UNIT=7,FMT=* ) n, nz
      if ( nz.gt.nzmax ) stop 'Please increase nzmax.'
      if ( n.gt.nmax ) stop 'Please increase nmax.'
      read( UNIT=7,FMT=* ) (ia(i),ja(i),a(i), i=1,nz)
      close( UNIT=7 )

*---  compute the infinite norm of A ...
      do i = 1,n
         wsp(i) = ZERO
      enddo
      do i = 1,nz
         wsp(ia(i)) = wsp(ia(i)) + ABS( a(i) )
      enddo
      anorm = wsp(1)
      do i = 2,n
         if ( anorm.lt.wsp(i) ) anorm =  wsp(i)
      enddo
      write(UNIT=*,FMT='(A,E8.2)') '||A||_inf= ',anorm
      
c---  Scale rate matrix to a probability transition matrix
c      maxdiag = 0
c      do i = 1, nz
c         if (ia(i).eq.ja(i)) then
c            if (abs(a(i)) > maxdiag) then
c                maxdiag = abs(a(i))
c            endif
c         endif
c      enddo
c      maxdiag = maxdiag * 1.1;
c      do i = 1, nz
c         a(i) = a(i) / maxdiag
c      enddo
c      do i = 1, nz
c         if (ia(i).eq.ja(i)) then
c            a(i) = a(i) + 1.0
c         endif
c      enddo
c---

*---  the operand vector v is set to (1, ..., 1)^T ...
      if (p0 == 1) then
        do i = 1,n
           v(i) = ZERO
        enddo
        v(1) = 1.0
      else if (p0 == 0) then
        do i = 1,n
           v(i) = 1.0 / real(n)
        enddo
      else if (p0 == 2) then
        open(UNIT=7,STATUS='old',IOSTAT=iflag,FILE=p0file)
        read(UNIT=7,FMT=*) v(1)
        read(UNIT=7,FMT=*) (v(i), i=1,n)
        close(7)
      endif

*---  set other input arguments ...
      nsteps = time/dt;
      tol = 1d-17
      m = 30
      itrace = 0

*---  compute w = exp(t*A)v with COO format ...
      tic = clock()
      write (dynfn, "(A,A4,I4.4,A4)") TRIM(dyndir), "/dyn", 0, ".txt"
      open(UNIT=8,STATUS='replace',IOSTAT=iflag,FILE=dynfn)
      write (8,9001), '1'
      do j = 1,n
        write (8,*) v(j)
      enddo
      close(8)

      t = 0;
      do i = 1,nsteps
        call DGEXPV( n, m, dt,v,w, tol, anorm,
     .             wsp,lwsp, iwsp,liwsp, dgcoov, itrace, iflag )
        do j = 1,n
          v(j) = w(j)
        enddo

        write (dynfn, "(A,A4,I4.4,A4)") TRIM(dyndir), "/dyn", i, ".txt"
        open(UNIT=8,STATUS='replace',IOSTAT=iflag,FILE=dynfn)
        write (8,9001), '1'
        do j = 1,n
          write (8,*) v(j)
        enddo
        close(8)

        t = t + dt;
        if (t >= time) then
          exit
        endif
      enddo
      tac = clock()

*---
c      print 9001,'----------------------------------------------------'
c      print 9001,'DGEXPV (COO) has completed:'
c      print 9001,'----------------------------------------------------'
c      print 9001,'w(1:n) ='

*---  display some statistics if desired ...
c      print 9001,'final report----------------------------------------'
c      print 9002,'runtime   = ',tac-tic
c      print 9002,'||A||_inf = ',anorm
c      print 9003,'nz        =',nz
c      print 9003,'n         =',n
c      print 9003,'m         =',m
c      print 9003,'itrace    =',itrace
c      print 9003,'iflag     =',iflag
c      print 9003,'ibrkflag  =',iwsp(6)
c      print 9003,'mbrkdwn   =',iwsp(7)
c      print 9003,'nstep     =',iwsp(4)
c      print 9003,'nreject   =',iwsp(5)
c      print 9003,'nmult     =',iwsp(1)
c      print 9003,'nexph     =',iwsp(2)
c      print 9003,'nscale    =',iwsp(3)
cc
c      print 9002,'tol       = ',tol
c      print 9002,'t         = ',t
c      print 9002,'tbrkdwn   = ',wsp(7)
c      print 9002,'step_min  = ',wsp(1)
c      print 9002,'step_max  = ',wsp(2)
c      print 9002,'max_round = ',wsp(3)
c      print 9002,'sum_round = ',wsp(4)
c      print 9002,'max_error = ',wsp(5)
c      print 9002,'sum_error = ',wsp(6)
c      print 9002,'hump      = ',wsp(9)
c      print 9002,'scale-norm= ',wsp(10)

*----------------------------------------------------------------------|
*----------------------------------------------------------------------|

 9001 format(A)
 9002 format(A,E8.2)
 9003 format(A,I9)
      contains
*----------------------------------------------------------------------|
      subroutine printUsage()
      
      implicit none
      
      print *,''
      print *,'Usage:'
      print *,' -m  --tm <matrixfile>  Input rate matrix file'
      print *,' -t  --time 100         Total time for time evolution'
      print *,' -d  --dt 10            Time step'
      print *,' -p  --p0 <p0file>      Initial distribution file.'
      print *,'          0             Uniform distribution'
      print *,'          1             1-probability at the first state'
      print *,'          p0file        Initial distribution file'
      print *,' -i  --idir <./>        Intermediate directory'
      print *,''
      
      END
      END program mxexp
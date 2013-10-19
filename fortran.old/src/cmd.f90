module cmd
  use global
  implicit none
  character(len=max_file_path_len) :: cwd ! current working path
  character(len=max_cmd_option_len),dimension(:),allocatable::args
  integer(kind=4)::argc ! command line args
  integer(kind=4),private::argc_n ! local counter
  contains
    subroutine cmd_end()
      implicit none
      logical(kind=logical_byte),save::end_stat=.false.
      if (end_stat .eqv. .true.) return
      end_stat=.true.
      write(*,*) "cmd_end() begins"
      call global_end()
      deallocate(args)
      write(*,*) "cmd_end() ends"
    end subroutine cmd_end
    subroutine cmd_init()
      implicit none
      logical(kind=logical_byte),save::init_stat=.false.
      if (init_stat .eqv. .true.) return
      init_stat=.true.
      write(*,*) "cmd_init() begins"
      call global_init()
      call getcwd(cwd) ! get current working directory
      argc=iargc() ! get number of arguments
      allocate(args(argc))
      do argc_n=1,argc ! get list of arguments
        call getarg(argc_n,args(argc_n))
      end do
      write(*,*) "cwd=",cwd
      write(*,*) "argc=",argc
      write(*,*) "args=",args
      write(*,*) "cmd_init() ends"
    end subroutine cmd_init
end module cmd

      program main
        use global
        use cmd
        use inputs
        use zdotx2
        implicit none
        complex(kind=byte),dimension(:),allocatable::x,y,V
        integer(kind=byte)::n,n_max
        call cmd_init() ! read in cmd arguments
        call zdotx2_init()
        !call zdotx2_test()
        allocate(V(Ng))
        phis=0.0
        if (argc>=1) read(args(1),*) phis ! default phis=0.0
        call plwvin(phis,V)
        n_max=3
        if (argc>=2) read(args(2),*) n_max
        allocate(x(Ns),y(Ns))

        x=V
        write(*,*) V(1:2)
        do n=1,n_max
          call dot2(x,y)
          write(*,*) x(1:2)
          x=y
        end do
        deallocate(x,y,V)
        
        call zdotx2_end()
        call cmd_end()
      end program main

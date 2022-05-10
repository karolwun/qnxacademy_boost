#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <vector>

struct Data
{
    int a;
    int b;
    Data(int a, int b) : a(a), b(b) {}
};

using namespace boost::interprocess;

typedef allocator<Data, managed_shared_memory::segment_manager> ShmemAllocator;

// Example is a modification of https://www.boost.org/doc/libs/1_63_0/doc/html/interprocess/quick_guide.html
int main(int argc, char **argv)
{
    if (argc == 1)
    { // Parent process
        // Remove shared memory on construction and destruction
        struct shm_remove
        {
            shm_remove() { remove(); }
            ~shm_remove() { remove(); }
            void remove() 
            {
                shared_memory_object::remove("SharedMemory");
            }
        } remover;

        // Create a managed shared memory segment
        const size_t seg_size = 65536;
        managed_shared_memory segment(create_only, "SharedMemory", seg_size);

        // Allocate a portion of the segment (raw memory) and save data
        managed_shared_memory::size_type free_memory = segment.get_free_memory();
        void *shptr = segment.allocate(1024 /*bytes to allocate*/);
        Data d(2,4);
        memcpy(shptr, &d, sizeof(d));

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);


        // Or create an objects or arrays on segments
        segment.construct<Data>("Data obj")(3, 6);
        int a_data [] = {5, 6, 7};
        int b_data [] = {10, 12, 14};
        segment.construct_it<Data>("Data arr")[3](&a_data[0], &b_data[0]);


        // Or Create vector inside shared memory
        //Initialize shared memory STL-compatible allocator
        const ShmemAllocator alloc_inst (segment.get_segment_manager());
        std::vector<Data, ShmemAllocator> *vec = 
            segment.construct<std::vector<Data, ShmemAllocator>>("Data vec")(alloc_inst);

        for (int i = 0; i < 100 ; i+=2)
        {
            vec->emplace_back(i, i + 1);
        }

        // Create command and launch child process with handle as a parameter
        std::stringstream s;
        s << argv[0] << " " << handle;
        s << std::ends;
        std::cout << s.str() << "\n";
        if (0 != std::system(s.str().c_str()))
            return 1;

        // Check memory has been freed
        if (free_memory != segment.get_free_memory()) {
            std::cout << "segment not freed \n";
            return 1;
        }
        // Check if named segments were deleted
        if (segment.find<Data>("Data obj").first || segment.find<Data>("Data arr").first)
        {
            std::cout << "object segments not freed \n";
            return 1;
        };
        //Check child has destroyed the vector
        if(segment.find<std::vector<Data, ShmemAllocator>>("Data vec").first)
        {
            std::cout << "vector segments not freed \n";
            return 1;
        }
         
    }
    else
    {
        // Open managed segment
        managed_shared_memory segment(open_only, "SharedMemory");

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = 0;

        // Obtain handle value
        std::stringstream s;
        s << argv[1];
        s >> handle;

        // Get buffer local address from handle
        void *msg = segment.get_address_from_handle(handle);

        // Get Data from raw buffer
        Data *d = (Data *)msg;
        std::cout << "raw data received: " << d->a << " " << d->b << "\n";

        // Get Data from objects
        std::pair<Data *, managed_shared_memory::size_type> seg_obj = segment.find<Data>("Data obj");
        auto seg_arr = segment.find<Data>("Data arr");

        std::cout << "obj received: " << seg_obj.first->a << " " << seg_obj.first->b << " cnt: " << seg_obj.second << "\n";
        std::cout << "arr received: " << seg_arr.first[1].a << " " << seg_arr.first[1].b << " cnt: " << seg_arr.second << "\n";

        //Get Data from vector
        std::vector<Data, ShmemAllocator> *vec = segment.find<std::vector<Data, ShmemAllocator>>("Data vec").first;
        std::cout << "vec received: " << vec->at(0).a << " " << vec->at(0).b << " " << vec->at(20).a << " " << vec->at(20).b 
            << " vecsize: " << vec->size() << "\n";

        // Deallocate previously allocated memory
        segment.deallocate(msg);
        segment.destroy<Data>("Data obj");
        segment.destroy<Data>("Data arr");
        segment.destroy<std::vector<Data, ShmemAllocator>>("Data vec");
    }

    return 0;
}
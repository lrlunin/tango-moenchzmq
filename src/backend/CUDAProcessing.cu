#include <cuda_runtime.h>
#include <cuda.h> 
#include <cooperative_groups.h>

#include <iostream>
#include <memory>
#include <string>
#include <random>

constexpr int cluster_size = 3;
constexpr int block_size = 22;
constexpr int grid_size = 20;
constexpr int nsigma = 1;
constexpr int c2 = (cluster_size + 1) / 2;
constexpr int c3 = cluster_size;
constexpr int BUFFER_SIZE = 100;
constexpr int IMAGE_HEIGHT = 400;
constexpr int IMAGE_WIDTH = 400;
constexpr int IMAGE_SIZE = IMAGE_HEIGHT * IMAGE_WIDTH;

__global__ void process_group2D(int* raw_frame, int* class_output_array, float* pedestal_sum, float* pedestal_squared_sum, int* pedestal_counter, bool isPedestal, int *blockIds, int* threadIds){
    
    int pixel_rms = 0;
    // for the most left and most right block we need take only one extra pixel from either right or left
    // for the blocks inbetween we need to take both left and right extra overlapping pixels  
    __shared__ int no_bkgd_for_block[block_size * block_size];
    // depending on the block position they can be unmodified
    // so need set them explictly for 0 !!! ONCE IN BLOCK !!!
    int local_x = threadIdx.x; // in [0, 1, 2, ... , 20, 21]
    int local_y = threadIdx.y; // in [0, 1, 2, ... , 20, 21]
    int global_x = (threadIdx.x + gridDim.x * blockIdx.x) - 1; // 20 * [0, 1, 2, 3, ... , 19] -> global x maps 0 ... 399
    int global_y = (threadIdx.y + gridDim.y * blockIdx.y) - 1; // 20 * [0, 1, 2, 3, ... , 19] -> global y maps 0 ... 399
    // top left column is (threadIdx.x == 0 && blockIdx.x == 0)
    // top row is (threadIdx.y == 0 && blockIdx.y == 0)
    // top right column is (threadIdx.x == (block_size - 1) && blockIdx.x == (grid_size - 1))
    // bottom row is (threadIdx.y == (block_size - 1) && blockIdx.y == (grid_size - 1))
    if (blockIdx.x == 0 && threadIdx.x == 0   ||
        (threadIdx.y == 0 && blockIdx.y == 0) ||
        (threadIdx.x == (block_size - 1) && blockIdx.x == (grid_size - 1)) ||
        (threadIdx.y == (block_size - 1) && blockIdx.y == (grid_size - 1))
       ){
        no_bkgd_for_block[local_y * block_size + local_x] = 0;
    } else {
        // avoid 0 division
        int pixel_counter = max(pedestal_counter[global_y * 400 + global_x], 1);
        int pixel_offset = pedestal_sum[global_y * 400 + global_x] / pixel_counter;
        pixel_rms = sqrt(pedestal_squared_sum[global_y * 400 + global_x] / pixel_counter - pow(pixel_offset, 2));
        no_bkgd_for_block[local_y * block_size + local_x] = raw_frame[global_y * 400 + global_x] - pixel_offset;
    }
    __syncthreads();

    // zones for overlapping (border of the blocks) 
    if (threadIdx.x == 0 ||
        threadIdx.x == (block_size - 1) ||
        threadIdx.y == 0 ||
        threadIdx.y == (block_size - 1)) return;

    // we still need to update pedestal in the global memory
    // actually we calculated each overlapping pixel twice but
    // need to update it only once
    // so we update it from the block where the given pixel is the most right => where this corresponds to 21th pixel

    int v, tot, tl, tr, bl, br, max_value = 0;
    int pixel_class = 0;
    // considering the global_x we are now inspecting the if a pixel global_x a photon_max -> etc
    // 0 - pedestal, 1- photon,  2 - photon_max, 3 - negative_pedestal
    if (!isPedestal){
        for (int dy = - cluster_size / 2; dy < cluster_size / 2 + 1; dy++){
            for (int dx = - cluster_size / 2; dx < cluster_size / 2 + 1; dx++){
                v = no_bkgd_for_block[(local_y + dy) * block_size + (local_x + dx)];
                tot += v;
                if (dx <= 0 && dy <=0) tl+= v;
                if (dx <= 0 && dy >=0) bl+= v;
                if (dx >= 0 && dy <=0) tr+= v;
                if (dx >= 0 && dy >=0) br+= v;
                if (v > max_value) max_value = v;
            }
        }
        // considering negative pedestal 
        if (no_bkgd_for_block[local_y * block_size + local_x] < -nsigma * pixel_rms) {
                class_output_array[global_y * 400 + global_x] = 3;
            // considering candidates which are some kind of photons (either partial photon charges or the center ) 
            } else if (max_value > nsigma * pixel_rms ||
                       // max value of clusters
                       max(tl, max(bl, max(tr, br))) > c2 * nsigma * pixel_rms ||
                       tot > c3 * nsigma * pixel_rms){
                class_output_array[global_y * 400 + global_x] = 3;
                // if the pixel candidate is a max_value in the area
                if (no_bkgd_for_block[local_y * block_size + local_x] == max_value){
                    class_output_array[global_y * 400 + global_x] = 2;
                }
            }
    }
    // since we updating only pixels on the right side we ignore the 0th pixel and 21th pixel in the last block
    // if the pixel_class remained unchanged and not associated with any kind of photons
    // it is a pedestal and need to be added to stats
    if (pixel_class == 0) {
        blockIds[global_x] = blockIdx.x;
        threadIds[global_x] = threadIdx.x;
        int pixel_counter = pedestal_counter[global_y * 400 + global_x]++;
        if (pixel_counter < BUFFER_SIZE){
            pedestal_sum[global_y * 400 + global_x] += raw_frame[global_y * 400 + global_x];
            pedestal_squared_sum[global_y * 400 + global_x] += pow(raw_frame[global_y * 400 + global_x], 2);
        }
        else {
            pedestal_sum[global_y * 400 + global_x] += raw_frame[global_y * 400 + global_x] - pedestal_sum[global_y * 400 + global_x] / BUFFER_SIZE;
            pedestal_squared_sum[global_y * 400 + global_x] += pow(raw_frame[global_y * 400 + global_x], 2) - pedestal_squared_sum[global_y * 400 + global_x] / BUFFER_SIZE;
        }
    }
}

int main(){
    int* test_array_h = new int[IMAGE_SIZE];
    int* output_array_h = new int[IMAGE_SIZE];
    int* input_array_d;
    int* output_array_d;
    float* pedestal_sum_h = new float[IMAGE_SIZE];
    float* pedestal_squared_h = new float[IMAGE_SIZE];
    int* pedestal_counter_h = new int[IMAGE_SIZE];
    int* threadIds_h = new int[IMAGE_SIZE];
    int* blockIds_h = new int[IMAGE_SIZE];

    float* pedestal_sum_d;
    float* pedestal_squared_d;
    int* pedestal_counter_d;
    int* threadIds_d;
    int* blockIds_d;
    cudaMalloc(&input_array_d, IMAGE_SIZE*sizeof(int));
    cudaMalloc(&output_array_d, IMAGE_SIZE*sizeof(int));
    cudaMalloc(&pedestal_sum_d, IMAGE_SIZE*sizeof(float));
    cudaMalloc(&pedestal_squared_d, IMAGE_SIZE*sizeof(float));
    cudaMalloc(&pedestal_counter_d, IMAGE_SIZE*sizeof(int));
    cudaMalloc(&threadIds_d, IMAGE_SIZE*sizeof(int));
    cudaMalloc(&blockIds_d, IMAGE_SIZE*sizeof(int));

    cudaMemcpy(input_array_d, test_array_h, IMAGE_SIZE*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(output_array_d, output_array_h, IMAGE_SIZE*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemset(pedestal_sum_d, 0, IMAGE_SIZE*sizeof(float));
    cudaMemset(pedestal_squared_d, 0, IMAGE_SIZE*sizeof(float));
    cudaMemset(pedestal_counter_d, 0, IMAGE_SIZE*sizeof(int));
    cudaMemset(threadIds_d, 0, IMAGE_SIZE*sizeof(int));
    cudaMemset(blockIds_d, 0, IMAGE_SIZE*sizeof(int));
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution d{100., 10.0};
    // seems like threads per block need to be + (cluster_size / 2) to blocks per grid
    for (int i = 0; i<200; i++){
        for (int y = 0; y < IMAGE_HEIGHT; y++){
            for (int x = 0; x < IMAGE_WIDTH; x++){
                int value = std::round(d(gen));
                test_array_h[y * IMAGE_HEIGHT + x] = value;
            }
        }
        cudaMemcpy(input_array_d, test_array_h, IMAGE_SIZE*sizeof(int), cudaMemcpyHostToDevice);
        // 22 threads per block is block size + (cluster_size - 1)
        dim3 threadsPerBlock(22, 22);
        dim3 blocksPerGrid(20, 20);
        process_group2D<<<blocksPerGrid, threadsPerBlock>>>(input_array_d, output_array_d, pedestal_sum_d, pedestal_squared_d, pedestal_counter_d, true, blockIds_d, threadIds_d);
    }
    cudaMemcpy(pedestal_sum_h, pedestal_sum_d, IMAGE_SIZE*sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(pedestal_squared_h, pedestal_squared_d, IMAGE_SIZE*sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(pedestal_counter_h, pedestal_counter_d, IMAGE_SIZE*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(blockIds_h, blockIds_d, IMAGE_SIZE*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(threadIds_h, threadIds_d, IMAGE_SIZE*sizeof(int), cudaMemcpyDeviceToHost);

    for (int x = 0; x<41; x++){
        printf("pedestal_sum %f, pedestal_sum_sqaured %f, counter %d, blockId %d, threadId %d\n", pedestal_sum_h[x], pedestal_squared_h[x], pedestal_counter_h[x], blockIds_h[x], threadIds_h[x]);
    }
    return 0;
}

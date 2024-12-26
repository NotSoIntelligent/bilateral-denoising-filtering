# Bilateral Denoising Filtering

Bilateral Filtering is a noise filtering technique that combines **domain filtering** and **range filtering** to preserve edges while reducing noise.

## Key Concepts

### Domain Filtering
Domain filtering averages image values with weights that decay with distance. It performs low-pass filtering by averaging the pixels in the neighborhood. In domain filtering, the weight assigned to each neighboring pixel depends on the Euclidean distance between the central pixel (the pixel on which the filter is applied) and the neighboring pixel.

This operation is mathematically expressed as:

Domain filtering weight = exp(-distance^2 / (2 * sigmaD^2))

Where `distance` is the Euclidean distance between pixels and `sigmaD` is a parameter controlling the degree of smoothing.

### Range Filtering
Range filtering averages image values with weights that decay based on the difference in pixel values (i.e., pixel dissimilarity). Range filtering, on its own, can distort an image's color map as it does not consider the spatial locality of the pixels.

The weight for range filtering is computed as:

Range filtering weight = exp(-colorDifference^2 / (2 * sigmaR^2))

Where `colorDifference` is the difference between pixel intensities and `sigmaR` controls the range of smoothing based on pixel similarity.

### Bilateral Filtering
Bilateral filtering is the combination of both domain and range filtering. For each pixel, the bilateral filter assigns a weight based on both its spatial distance and its color similarity to the central pixel.

The final bilateral filter weight is calculated as:

Bilateral filtering weight = (Range filtering weight) * (Domain filtering weight)

## Chroma and Luma Bilateral Filtering

- **Chroma Bilateral Filtering**: Applies bilateral filtering to the *U* and *V* planes of the input YUV420p image (chrominance channels).
- **Luma Bilateral Filtering**: Applies bilateral filtering to the *Y* plane of the input YUV420p image (luminance channel).

## Steps to Run the Program

1. **Set Filter Parameters**:
   - `sigmaR` (Range filter parameter)
   - `sigmaD` (Domain filter parameter)
   - `neighbourLimit` (Controls the maximum neighborhood considered for filtering)

2. **Set Input Image Path**:
   Provide the path to the input YUV420p image:
   - Example: `input_path = "D://input_image.yuv"`

3. **Set Output Image Path**:
   Provide the desired output YUV420p image path:
   - Example: `output_path = "D://chroma_bilateral_filtered_img.yuv"`
   - Note: Ensure that the output file name ends with the `.yuv` extension. The output file will be created at the specified location.

---

By following the above steps, you can apply bilateral filtering to YUV420p images, reducing noise while preserving edges.




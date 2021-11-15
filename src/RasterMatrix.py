import rasterio, geopandas

filepath = "/home/qweddww123/farsite/examples/Panther/test1177973/input/test1177973.lcp"

# Open the file:
raster = ratserio.open(filepath)

# Read the raster band as separate variable
band = raster.read(1)

# Print only selected metadata:
print(mat)
print(mat[0])
print(mat[0][0])


import geopandas as gpd


def shapefile_to_txt(input_shp, output_txt):
    # 读取Shapefile
    gdf = gpd.read_file(input_shp)

    # 打开输出文本文件
    with open(output_txt, 'w') as output_file:
        # 写入字段名
        field_names = gdf.columns.tolist()
        output_file.write('\t'.join(field_names) + '\n')

        # 写入要素属性值
        for idx, row in gdf.iterrows():
            values = [str(value) for value in row]
            output_file.write('\t'.join(values) + '\n')

    print('转换完成！')


# 指定输入Shapefile路径和输出txt路径
input_shp = 'C:/Users/pqs-xsz-xia/Desktop/Beijing_con.shp'
output_txt = 'C:/Users/pqs-xsz-xia/Desktop/b15.txt'

# 调用函数进行转换
shapefile_to_txt(input_shp, output_txt)

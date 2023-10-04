from jinja2 import Environment, FileSystemLoader

# 加载模板
env = Environment(loader=FileSystemLoader('.'))
template = env.get_template('template.html')

# 生成数据
lng1 = 116.399
lat1 = 39.910
lng2 = 116.405
lat2 = 39.920
lng3 = 116.425
lat3 = 39.900
lng4 = 116.415
lat4 = 39.905
# 将数据注入模板并呈现HTML页面
output = template.render(lat1=lat1, lng1=lng1, lat2=lat2, lng2=lng2,lat3=lat3, lng3=lng3, lat4=lat4, lng4=lng4)
with open('map.html', 'w', encoding='utf-8') as f:
    f.write(output)


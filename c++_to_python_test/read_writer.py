import re

# with open('C:/Users/pqs-xsz-xia/Desktop/B/b33.txt', 'r') as infile:
#     # 打开目标文件
#     with open('C:/Users/pqs-xsz-xia/Desktop/B/b4.txt', 'w') as outfile:
#         # 逐行读取原始文件
#         for i, line in enumerate(infile):
#             # 将数字i先写入目标文件
#             outfile.write(str(i + 1) + '\n')
#             # 将该行内容写入目标文件
#             outfile.write(line)
# # 关闭文件
# infile.close()

#

def fill_right(s, length, fill_char):
    return s.ljust(length, fill_char)


# with open('C:/Users/pqs-xsz-xia/Desktop/B/b13.txt', 'r') as f_in, open('C:/Users/pqs-xsz-xia/Desktop/B/b14.txt', 'w') as f_out:
#     for line in f_in:
#         count = line.count(' ')
#         number = line.split()[0]
#         elements = line.split()
#         content = " ".join(elements[1:])
#         if count==1:
#             f_out.write(number + ' ' + fill_right(content,33,'*') + '\n')
#         else:
#             f_out.write(line)

with open('C:/Users/pqs-xsz-xia/Desktop/B/b14.txt', 'r') as f_in:
    for line in f_in:
        if len(line)!=40:
            print('有情况！')



# count=0
# c=1
# i=1
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b10.txt', 'r') as f_in, open('C:/Users/pqs-xsz-xia/Desktop/B/b11.txt', 'w') as f_out:
#     for line in f_in:
#         number = line.split()[0]
#         elements = line.split()
#         content = " ".join(elements[1:])
#         if float(number) == i:
#             f_out.write(line)
#             c=1
#             count=content
#             i = i + 1
#         else:
#             f_out.write(f'{str(c).zfill(5)}'+' '+'00000000000000'+' '+line)
#             c=c+1
#

#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b10.txt', 'r') as f:
#     lines = f.readlines()  # 读取原始文本的每一行
#
# i = 0
#
# ii=1
# new_lines = []  # 存储新的文本行
#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b10.txt', 'w') as f:
#     for i in new_lines:
#         f.write(f'{i}\n')  # 将新的文本写入文件
#
#     while i < len(lines):
#         if ' ' in  lines[i]:  # 发现一个新的序号
#             j = i  #1
#             while j < len(lines) and ' ' in lines[j]:  # 统计该序号下的坐标数量
#                 j += 1
#             count = j - i-1  # 坐标数量等于序号和下一个序号之间的行数
#             f.write(f"{lines[i-1].strip() + ' ' + str(count+1)}\n")  # 在序号后面添加坐标数量
#             for k in range(i,j):
#                 f.write(f'{lines[k]}')  # 添加该序号下的所有坐标
#             i = j
#         else:
#             i=i+1

#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b11.txt', 'r') as f:
#     lines = f.readlines()  # 读取原始文本的每一行
# new_lines = []  # 存储新的文本行
#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b12.txt', 'w') as f:
#     i=0
#     while i < len(lines):
#         count = lines[i].count(' ')
#         number = lines[i].split()[0]
#         elements = lines[i].split()
#         content = " ".join(elements[1:])
#         if count==1:
#             if int(content)==2 or int(content)==3:
#                 i=i+int(content)+1
#             else:
#                 f.write(f'{lines[i]}')
#                 i = i + 1
#         else:
#             f.write(f'{lines[i]}')
#             i=i+1
#

#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b12.txt', 'r') as f:
#     lines = f.readlines()  # 读取原始文本的每一行
# new_lines = []  # 存储新的文本行
#
# with open('C:/Users/pqs-xsz-xia/Desktop/B/b13.txt', 'w') as f:
#     i=0
#     ii=1
#     while i < len(lines):
#         count = lines[i].count(' ')
#         number = lines[i].split()[0]
#         elements = lines[i].split()
#         content = " ".join(elements[1:])
#         if count==1:
#                 f.write(str(ii).zfill(5)+' '+content+'\n')
#                 i = i + 1
#                 ii=ii+1
#         else:
#             f.write(f'{lines[i]}')
#             i=i+1
#
#


# line = "00001 39.90626"
# number = line.split()[0]
# elements = line.split()
# content = " ".join(elements[1:])
#
# print(content)

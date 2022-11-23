src=$(wildcard ./*c)
objs=$(patsubst %.c, %.o, $(src))
target=app

$(target):$(objs)
	$(CC) $(objs) -o $(target)

#上面缺少文件时，会来下面的规则中找。
#上面缺少sub.o 就会来下面找，%匹配的sub
%.o:%.CC
	$(CC) -c $< -o $@

clean:
	rm $(objs) -f
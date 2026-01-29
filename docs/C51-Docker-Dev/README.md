# C51 Docker 开发环境

## 功能
- 基于 summit4you/51scm:sdcc-3.8-stcgal-1.6
- 内置非 root 用户 `c51`
- 挂载本地 C51 工程目录
- 支持 SDCC 编译和 stcgal 烧录

## 使用方法

### 构建镜像
```bash
docker-compose build
```

### 启动容器
```bash
docker-compose up -d
docker exec -it C51 bash
```

### 挂载项目
默认挂载宿主机 `/home/enoch/Docker/C51` 到容器 `/home/c51/Docker/C51`

### 串口设备
如果需要烧录板子，取消 `docker-compose.yml` 中对应的 `volumes` 串口挂载

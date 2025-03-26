FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    mpich \
    python3 \
    python3-pip \
    openssh-server \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install numpy

# Configure SSH
RUN mkdir /var/run/sshd
RUN echo 'root:mpi' | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#StrictHostKeyChecking yes/StrictHostKeyChecking no/' /etc/ssh/ssh_config

RUN ssh-keygen -t rsa -f /root/.ssh/id_rsa -N ""
RUN cat /root/.ssh/id_rsa.pub > /root/.ssh/authorized_keys

WORKDIR /app

COPY . .

RUN python3 generate_data.py

RUN mpicc -fopenmp parallel_processing.c -o parallel_processing -lm

RUN chmod +x start_mpi.sh

EXPOSE 22

CMD service ssh start && ./start_mpi.sh
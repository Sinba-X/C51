FROM summit4you/51scm:sdcc-3.8-stcgal-1.6
ARG USERNAME=c51
ARG UID=1000
ARG GID=1000
RUN groupadd -g ${GID} ${USERNAME}     && useradd -m -u ${UID} -g ${GID} -s /bin/bash ${USERNAME}     && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER ${USERNAME}
WORKDIR /home/${USERNAME}
CMD ["/bin/bash"]

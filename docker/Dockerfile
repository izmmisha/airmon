FROM espressif/idf

ENV CCACHE_DIR=/tmp/.ccache

RUN cd $IDF_PATH/components/mbedtls/mbedtls && \
    git fetch && \
    git checkout -b mbedtls-2.16.6-adk origin/mbedtls-2.16.6-adk


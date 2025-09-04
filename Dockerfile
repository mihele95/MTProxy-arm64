FROM alpine:3.6 as build

WORKDIR /src

RUN apk add --no-cache --virtual .build-deps \
      git make gcc musl-dev linux-headers openssl-dev curl perl

RUN curl -OL https://www.openssl.org/source/old/1.0.2/openssl-1.0.2u.tar.gz \
    && tar -xzf openssl-1.0.2u.tar.gz \
    && cd openssl-1.0.2u \
    && ./config no-shared no-zlib --prefix=/usr/local/openssl-static \
    && make -j$(nproc) \
    && make install

COPY . .

RUN make -j$(nproc)

FROM alpine:3.6

COPY --from=build /src/objs/bin/mtproto-proxy /usr/local/bin/mtproto-proxy

ENTRYPOINT ["/usr/local/bin/mtproto-proxy"]

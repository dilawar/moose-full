
def normalizer(minscalar, maxscalar, clipleft=True, clipright=True):
    return lambda scalar: normalize(scalar,
                                    minscalar,
                                    maxscalar,
                                    clipleft,
                                    clipright)


def normalize(scalar, minscalar, maxscalar, clipleft=True, clipright=True):
    if clipleft and scalar < minscalar:
        scalar = minscalar
    if clipright and scalar > maxscalar:
        scalar = maxscalar
    return (scalar - minscalar) / (maxscalar - minscalar)


def mapper(transformer, normalizer):
    return lambda scalar: transformer(normalizer(scalar))

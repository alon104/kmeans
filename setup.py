from setuptools import setup, Extension


setup(
	name='mykmeanssp',
	version='1.0',
	description='a kmeans++ algorithm',
    ext_modules=[
        Extension(
            'mykmeanssp',
            ['kmeansmodule.c','kmeans.c'],
        )
    ]
)

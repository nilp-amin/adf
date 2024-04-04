from setuptools import find_packages, setup

package_name = 'proximity'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools',
                      "rclpy"],
    zip_safe=True,
    maintainer='nilp',
    maintainer_email='nilpamin2@gmail.com',
    description='Used to check if motion is detected in ADF',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'proximity_node = proximity.proximity_node:main'
        ],
    },
)

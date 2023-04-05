Byte Array
=======================================

The ByteArray is a class used for storing binary data. It helps the user working with binary data.

There are multiple ways of creating an Binary Array.

.. code-block:: c++
    :caption: creating the Binary Array
    :linenos:

    // at the top of the file
    #include <vrock/utils/ByteArray.hpp>
    
    // using constructor
    auto arr = vrock::utils::ByteArray("example data");
    // this will create a BinaryArray instance with the length of
    // 12 and the ASCII values of the individual characters
    // alternatively you can do the following
    auto arr1 = vrock::utils::ByteArray::from_hex_string("31323334");
    // this will read the string as hexadecimal and result in the following
    // values 49, 50, 51, 52 which would result in the following string -> '1234'

    // if you have a pointer to binary data, you can do the following
    auto arr2 = vrock::utils::ByteArray(len, data_ptr);
    
    // if you want a Binary Array with a known length but don't
    // have the data yet you can use
    auto arr3 = vrock::utils::ByteArray(len);

.. note:: `from_hex_string` and `from_string` return std::shared_ptr

getting a Byte at a specific position you can do the following.

.. code-block:: c++
    :caption: accessing the data
    :linenos:

    auto arr = vrock::utils::ByteArray("example data");
    // getting byte via the get function
    std::cout << arr.get(3) << std::endl;
    // or by using the data directly 
    std::cout << arr.data[4] << std::endl;
    // this will print the following
    // m
    // p

setting bytes is done in almost the same way.

.. code-block:: c++
    :caption: manipulating the data
    :linenos:

    auto arr = vrock::utils::ByteArray("example data");
    // setting a byte via the set function
    arr.set(3, 'p'); // this will change the data to 'exapple data'
    // or by using the data directly 
    arr.data[4] = 'm'; // this will change the data to 'exapmle data'
    


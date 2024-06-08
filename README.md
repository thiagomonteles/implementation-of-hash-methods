# Hash Table Implementations and Collision Handling

## Overview

This repository contains implementations and analyses of various hash functions and collision handling methods for hash tables. The goal is to compare the efficiency and performance of different hashing techniques and collision resolution strategies.

## Hash Functions

The following hash functions have been implemented and tested:

1. **Division Method**:
   - Generates the hash key by taking the remainder of the original key divided by the table size.

2. **Division Method with Prime Table Size**:
   - Similar to the division method but uses the smallest prime number greater than or equal to the table size.

3. **Folding Method**:
   - Sums the digits of the original key until the number of digits matches the table size. For keys in base 10, the table size should be a power of 10.

4. **Simple Multiplication Method**:
   - Multiplies the key by a constant between 0 and 1, discards the integer part of the result, and then multiplies the remainder by the table size.

5. **Squared Multiplication Method**:
   - Squares the key, multiplies by a constant between 0 and 1, discards the integer part of the result, and then multiplies the remainder by the table size.

## Collision Handling Methods

The following collision handling methods have been implemented:

1. **External Chaining with First Record Considered**:
   - The first record is stored directly in the table, with subsequent records that hash to the same value stored in a linked list.

2. **External Chaining with No Record Considered**:
   - All records that hash to the same value are stored in a linked list, with the table storing pointers to the heads of these lists.

3. **Collision Zone Method**:
   - Part of the table is dedicated to direct hashing, and the rest serves as a collision zone where collisions are resolved linearly.

## Files

The repository contains the following files:

1. **hash_collision_zone.c**:
   - Implementation of the collision zone method.

2. **hashideal_dobra.c**:
   - Implementation of the folding method.

3. **hashideal_primo.c**:
   - Implementation of the division method with prime table size.

4. **hashideal_rest.c**:
   - Implementation of the division method.

5. **series_hash.c**:
   - Implementation of the simple multiplication method.

6. **series_hash_1r.c**:
   - Implementation of the squared multiplication method.

## Report Summary

The report includes detailed analyses of each method's performance based on the number of collisions and the distribution of keys across the table.

### Key Findings:

- **Division Method**:
  - Processed 10,000 records with 6,321 successfully registered and 3,679 collisions.

- **Division Method with Prime Table Size**:
  - Processed 10,007 records (using the smallest prime greater than 10,000), with 6,298 successfully registered and 3,702 collisions.

- **Folding Method**:
  - Processed 10,000 records with 6,296 successfully registered and 3,704 collisions.

- **Simple Multiplication Method**:
  - Processed 10,000 records with 6,293 successfully registered and 3,707 collisions.

- **Squared Multiplication Method**:
  - Processed 10,000 records with 6,354 successfully registered and 3,646 collisions.

### Collision Handling:

- **External Chaining (First Record Considered)**:
  - Average operations: 1.344

- **External Chaining (No Record Considered)**:
  - Average operations: 2.338

- **Collision Zone**:
  - Average operations: 965.867981

## Conclusion

The implementations and analyses demonstrate the trade-offs between different hash functions and collision handling methods. The results highlight the importance of selecting an appropriate hashing strategy and collision resolution method based on the specific use case and data characteristics.

## References

- [Instituto de Matemática e Estatística | IME-USP](https://www.ime.usp.br/~hbolfar/aula_2013/Aula6-A12012.pdf)
- [DCA | FEEC - Faculdade de Engenharia Elétrica e de Computação - UNICAMP](http://www.dca.fee.unicamp.br/cursos/EA876/apostila/HTML/node26.html)
- [Instituto de Computação - UFF](http://www2.ic.uff.br/~boeres/slides_ed/ed_TabelaHash.pdf)
- [UFJF - Universidade Federal de Juiz de Fora](http://www.ufjf.br/jairo_souza/files/2012/11/4-Hashing-Fun%C3%A7%C3%B5es.pdf)
- [UFG - Universidade Federal de Goiás, Instituto de Informática](https://github.com/Professor-Fabrizzio/AED2/tree/master/Trabalho1)

---

For detailed implementation and further reading, refer to the files provided in this repository and the accompanying report.

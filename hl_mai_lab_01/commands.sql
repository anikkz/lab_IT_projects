# use root
# $ sudo mysql root

# if test user not exist
CREATE USER 'test'@'localhost' IDENTIFIED BY 'pzjqUkMnc7vfNHET';
# endif

CREATE DATABASE itprojects_labs;
GRANT ALL PRIVILEGES ON itprojects_labs.* TO 'test'@'localhost';
FLUSH PRIVILEGES;

#use test user
use itprojects_labs;
CREATE TABLE IF NOT EXISTS `Person` (`id` INT NOT NULL AUTO_INCREMENT, `login` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`age` TINYINT UNSIGNED NULL, PRIMARY KEY (`id`),UNIQUE KEY `login_hash` (`login`), KEY `fn` (`first_name`),KEY `ln` (`last_name`));

# load gen data
use itprojects_labs;
source gen_data100k.sql;


# other requests
describe Person;
explain format=json select * from Person LIMIT 0, 20;
explain select * from Person where first_name='Rhianna';
explain select * from Person where login='307-26-1464';
create index login_hash USING HASH  on Person(login);
drop index login_hash on Person;
explain select * from Person where age<20 AND age > 10;
create index age_hash USING BTREE  on Person(age);
drop index age_hash on Person;
show index from Person;
SELECT LAST_INSERT_ID();
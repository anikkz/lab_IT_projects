use sql_test;
DELETE FROM `Person`
-- sharding:0
;
ALTER TABLE `Person` AUTO_INCREMENT = 1
-- sharding:0
;
DELETE FROM `Person`
-- sharding:1
;
ALTER TABLE `Person` AUTO_INCREMENT = 1
-- sharding:1
;
DELETE FROM `Person`
-- sharding:2
;
ALTER TABLE `Person` AUTO_INCREMENT = 1
-- sharding:2
;

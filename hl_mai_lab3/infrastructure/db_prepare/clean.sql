use sql_test;
DROP TABLE IF EXISTS `Person`
-- sharding:0
;
DROP TABLE IF EXISTS `Person`
-- sharding:1
;
DROP TABLE IF EXISTS `Person`
-- sharding:2
;

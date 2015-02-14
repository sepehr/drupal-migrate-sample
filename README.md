#D6 to D7 Migration Sample
Even though that it's a personal repo, it may serve as a good starting point for those who are starting to write their own migration module.

##Scenario
The source for this migration was 4 separate Drupal 6 installations (per each available language!) containing more than 1000 product nodes, a few users, 4 taxonomy terms. The gotcha was that the genius D6 developer has used the [image_attach](http://drupal.org/project/image) module for product node images. As the results, each image has its own wrapping node which then was referenced in the target product node.

See `picture.inc` and `node.inc` which resolve these nodes into D7 files during the migration.

##Implementation
The migration module is written on top of [d2d](https://www.drupal.org/project/d2d) framework.
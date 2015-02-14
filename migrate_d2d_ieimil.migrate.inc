<?php
/**
 * @file migrate_d2d_ieimil.migrate.inc
 */

/**
 * Implements hook_migrate_api().
 */
function migrate_d2d_ieimil_migrate_api() {

  // --------------------------------------------------------------------------
  // Init
  // --------------------------------------------------------------------------
  $api = array(
    'api'        => 2,
    'migrations' => array(),
    'groups'     => array(
      'ieimil' => array(
        'title' => t('IEIMil Migrations (English)'),
      ),
    ),
  );

  $common_arguments = array(
    'source_connection' => 'legacy',
    'source_version'    => 6,
    'group_name'        => 'ieimil',

    'source_database'   => array(
      'driver'   => 'mysql',
      'database' => 'LEGACY_DATABSE_NAME_HERE',
      'username' => 'LEGACY_USER_WITH_READ_PERM',
      'password' => 'LEGACY_USER_PASSWORD',
      'host'     => 'localhost',
      'prefix'   => '',
    ),
  );

  // --------------------------------------------------------------------------
  // Users
  // --------------------------------------------------------------------------
  $api['migrations']['MigrateLegacyUsers'] = $common_arguments + array(
    'description'  => t('Migration of users from Drupal 6'),
    'class_name'   => 'DrupalUser6Migration',
  );

  // --------------------------------------------------------------------------
  // Terms
  // --------------------------------------------------------------------------
  $api['migrations']['MigrateLegacyTerms_Products'] = $common_arguments + array(
    'description'            => t('Migration of Product Category terms from Drupal 6'),
    'source_vocabulary'      => '1',  // Legacy Vocabulary ID
    'destination_vocabulary' => 'products',
    'class_name'             => 'DrupalTerm6Migration',
    'soft_dependencies'      => array('MigrateLegacyUsers'),
  );

  // --------------------------------------------------------------------------
  // Files
  // --------------------------------------------------------------------------
  $api['migrations']['MigrateLegacyAttachments'] = $common_arguments + array(
    'class_name'        => 'DrupalFile6Migration',
    'description'       => t('Import Drupal 6 files'),
    'user_migration'    => 'MigrateLegacyUsers',
    'default_uid'       => 1,
    'source_dir'        => '',
    'destination_dir'   => 'public://sites/default/files',
    'file_replace'      => MigrateFile::FILE_EXISTS_REUSE,
    'soft_dependencies' => array('MigrateLegacyTerms_Products'),
  );

  // --------------------------------------------------------------------------
  // Images from image_attach module
  // --------------------------------------------------------------------------
  $api['migrations']['MigrateLegacyImages'] = $common_arguments + array(
  	// See: picture.inc:
    'class_name'        => 'IeimilPictureMigration',
    'description'       => t('Import image_attach pictures'),
    'default_uid'       => 1,
    'source_dir'        => '',
    'destination_dir'   => 'public://sites/default/files/images',
    'file_replace'      => MigrateFile::FILE_EXISTS_REUSE,
    'soft_dependencies' => array('MigrateLegacyTerms_Products'),
  );

  // --------------------------------------------------------------------------
  // Nodes
  // --------------------------------------------------------------------------
  $api['migrations']['MigrateLegacyNodes_Products'] = $common_arguments + array(
  	// See: node.inc:
    'class_name'        => 'IeimilProductMigration',
    'description'       => t('Migration of Product nodes from Drupal 6'),
    'source_type'       => 'product',
    'destination_type'  => 'products',
    'dependencies'      => array('MigrateLegacyImages', 'MigrateLegacyAttachments'),
    // 'soft_dependencies' => array('MigrateLegacyAttachments'),
    'user_migration'    => 'MigrateLegacyUsers',
    'default_language'  => 'en',
  );

  return $api;
}

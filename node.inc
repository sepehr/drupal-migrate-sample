<?php

/**
 * Image-specific mappings and handling.
 */
class IeimilProductMigration extends DrupalNode6Migration {

  /**
   * Constructor
   *
   * @param array $arguments
   */
  public function __construct(array $arguments) {

    //
    // Add any other data we're pulling into the source row, before the parent constructor.
    //

    $this->sourceFields['fid'] = 'file id associated with image';

    parent::__construct($arguments);

    $this->dependencies[] = 'MigrateLegacyTerms_Products';
    $this->dependencies[] = 'MigrateLegacyImages';
    $this->dependencies[] = 'MigrateLegacyAttachments';

    $arguments = array('source_path' => 'public://sites/default/files/images');

    // language
    $this->addFieldMapping('language')->defaultValue('en');

    // field_arrangement
    $this->addFieldMapping('field_arrangement', 'field_product_arrange');

    // field_attachment
    $this->addFieldMapping('field_attachment', 'upload')->sourceMigration('MigrateLegacyAttachments');

    $this->addFieldMapping('field_attachment:file_class')->defaultValue('MigrateFileFid');

    $this->addFieldMapping('field_attachment:preserve_files')->defaultValue(TRUE);

    $this->addFieldMapping('field_attachment:description', 'upload:description');

    $this->addFieldMapping('field_attachment:display', 'upload:list');

    $this->addFieldMapping('field_attachment:file_replace', MigrateFile::FILE_EXISTS_REUSE);

    // $this->addFieldMapping('field_attachment:destination_file', 'filename');

    $this->addFieldMapping('field_my_image:language')->defaultValue(LANGUAGE_NONE);

    // field_image
    $this->addFieldMapping('field_image', 'fid')->arguments($arguments);
    $this->addFieldMapping('field_image:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_image:preserve_files')->defaultValue(TRUE);

    // field_category
    $this->addFieldMapping('field_category', '1')->sourceMigration('MigrateLegacyTerms_Products');
    $this->addFieldMapping('field_category:source_type')->defaultValue('tid');
  }

  // --------------------------------------------------------------------------

  /**
   * Implementation of Migration::prepareRow().
   *
   * Here we pull in additional data from a non-core, non-CCK table.
   *
   * @param $row
   *
   * @return bool
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Image query
    $pattern = "%{$row->nid}%";
    $image   = Database::getConnection('default', 'default')
      ->select('file_managed', 'f')
      ->fields('f', array('fid'))
      ->condition('filename', $pattern, 'LIKE')
      ->execute()
      ->fetchObject();

    // Debug
    // dpm($row);
    // dpm($image);

    if ( ! empty($image->fid)) {
      $row->fid = $image->fid;
    }

    else {
      $this->saveMessage(t('No corresponding image has been found for the nid:!nid', array('!nid' => $row->nid)));
    }

    return TRUE;
  }

  // --------------------------------------------------------------------------

}

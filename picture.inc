<?php
/**
 * Migrates D6 image_attach nodes to D7 image fields.
 */
class IeimilPictureMigration extends DrupalFile6Migration {

  /**
   * Constructor
   *
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    $this->sourceFields['filename'] = 'converted filename';

    parent::__construct($arguments);

    $this->addFieldMapping('destination_file', 'filename');
  }

  // --------------------------------------------------------------------------

  /**
   * Implementation of Migration::query().
   */
  protected function query() {
    $query = parent::query();

    $query->innerJoin('image', 'i', 'f.fid=i.fid');
    $query->condition('image_size', '_original');

    return $query;
  }

  // --------------------------------------------------------------------------

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('image_attach', 'ia')
      ->fields('ia', array('nid'));

    $query->innerJoin('image', 'i', 'ia.iid=i.nid');
    $query->condition('i.fid', $row->fid);
    $query->condition('image_size', '_original');
    $node = $query->execute()->fetchObject();

    if ($node) {
      $nid           = strval($node->nid);
      $pathinfo      = pathinfo($row->filepath);
      $filename      = $pathinfo['filename'];
      $extension     = $pathinfo['extension'];
      $row->filename = $nid . '.' . $extension;
    }
  }

  // --------------------------------------------------------------------------

}

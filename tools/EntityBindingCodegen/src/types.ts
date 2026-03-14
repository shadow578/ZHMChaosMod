export interface EntityProperty {
  name: string;
  type?: string;
}

export interface EntityPin {
  name: string;
  direction: "input" | "output";
  description?: string;
}

export interface EntityInfo {
  metadata: {
    name?: string;
    hash?: string;
    path?: string;
    parser: string;
  };
  properties: EntityProperty[];
  pins: EntityPin[];
}

export type EntityParserFunction = (data: object) => EntityInfo | undefined;

export interface PinDefinition {
  pin: string;
  description?: string;
}

export interface EntityPinDefinition {
  hash: string;
  path: string;
  in: PinDefinition[];
  out: PinDefinition[];
}

import { EntityInfo, EntityProperty, EntityPin, PinDefinition } from "./types";
import { PinDefinitionsHelper } from "./pindefs";

function generateProperty(property: EntityProperty): string {
  // "PROPERTY(TYPE, NAME)" if type is known
  // "//PROPERTY(?, NAME)" if type not known
  return `${!!property.type ? "" : "//"}PROPERTY(${property.type || "?"}, ${property.name});`;
}

function generatePin(pin: EntityPin, pinDef?: PinDefinition): string {
  // "INPUT_PIN(NAME)" or "OUTPUT_PIN(NAME)" depending on direction
  // If pinDef is provided, add a comment with the description for use with intellisense
  let code = `${pin.direction.toUpperCase()}_PIN(${pin.name});`;
  if (pinDef) {
    code += ` // ${pinDef.description}`;
  }

  return code;
}

function generateExposedInterface(iface: string): string {
  // "EXPOSED_INTERFACE(INTERFACE, NAME)"
  let name = iface;
  if (name.startsWith("I") || name.startsWith("Z")) {
    name = name.substring(1);
  }
  if (name.endsWith("Entity")) {
    name = name.substring(0, name.length - "Entity".length);
  }

  return `EXPOSED_INTERFACE(${iface}, ${name});`;
}

export function generate(
  entityInfo: EntityInfo,
  pinDefHelper: PinDefinitionsHelper,
): string {
  const pinDefs = pinDefHelper.findPinDefinition(
    entityInfo.metadata.hash || entityInfo.metadata.path || "",
  );

  const entityPath =
    entityInfo.metadata.path || pinDefs?.path || entityInfo.metadata.hash;

  let code = `#pragma once
#include "Entity/Bindings/EntityBinding.h"

// ${entityPath || ""}
struct S${entityInfo.metadata.name || "Entity"}Binding : public SEntityBinding
{
`;

  for (const property of entityInfo.properties) {
    code += `  ${generateProperty(property)}\n`;
  }
  code += "\n";

  for (const pin of entityInfo.pins.filter((p) => p.direction === "input")) {
    const def = pinDefs?.in.find((d) => d.pin === pin.name);
    code += `  ${generatePin(pin, def)}\n`;
  }
  for (const pin of entityInfo.pins.filter((p) => p.direction === "output")) {
    const def = pinDefs?.out.find((d) => d.pin === pin.name);
    code += `  ${generatePin(pin, def)}\n`;
  }
  code += "\n";

  for (const iface of entityInfo.exposedInterfaces) {
    code += `  ${generateExposedInterface(iface)}\n`;
  }
  code += "\n";

  code += "};\n";
  return code;
}
